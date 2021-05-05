#include <adm/utilities/block_duration_assignment.hpp>
#include <adm/helper/get_optional_property.hpp>
#include <adm/document.hpp>
#include <adm/errors.hpp>
#include <adm/route_tracer.hpp>
#include <memory>
#include <map>
#include <stdexcept>
#include <adm/utilities/time_conversion.hpp>

namespace adm {

  Time subtractTimes(const Time& firstTime, const Time& secondTime) {
    // both nanoseconds -> return nanoseconds
    if (firstTime.isNanoseconds() && secondTime.isNanoseconds())
      return firstTime.asNanoseconds() - secondTime.asNanoseconds();

    // both fractional with same denominator -> keep denominator
    if (firstTime.isFractional() && secondTime.isFractional()) {
      FractionalTime firstFrac = firstTime.asFractional();
      FractionalTime secondFrac = secondTime.asFractional();

      if (firstFrac.denominator() == secondFrac.denominator())
        return FractionalTime{firstFrac.numerator() - secondFrac.numerator(),
                              firstFrac.denominator()};
    }

    // mixed, or different denominators
    return asTime(asRational(firstTime) - asRational(secondTime));
  }

  bool timesEqual(const Time& firstTime, const Time& secondTime) {
    return firstTime.asFractional().normalised() ==
           secondTime.asFractional().normalised();
  }

  Time durationOfProgramme(const AudioProgramme* programme,
                           boost::optional<Time> fileLength) {
    if (programme->has<End>()) {
      auto duration = subtractTimes(programme->get<End>().get(),
                                    programme->get<Start>().get());
      // if a file length is given AND a programme end is set, both durations
      // must match
      if (fileLength && !timesEqual(fileLength.get(), duration)) {
        throw error::detail::formatElementRuntimeError(
            programme->get<AudioProgrammeId>(),
            "Programme length does not match specified filelength");
      }
      return duration;
    } else if (fileLength) {
      return fileLength.get();
    } else {
      throw error::detail::formatElementRuntimeError(
          programme->get<AudioProgrammeId>(),
          "Cannot calculate duration from programme without End nor "
          "filelength");
    }
  }

  Time durationOfChannel(const Route& route, Time fileLength) {
    assert(
        isVariantType<std::shared_ptr<const AudioChannelFormat>>(route.back()));
    auto lastObject = route.getLastOf<AudioObject>();
    auto effectiveDuration = getPropertyOr(lastObject, Duration(fileLength));
    return effectiveDuration.get();
  }

  template <typename BlockType>
  Duration calculateDuration(const BlockType& first, const BlockType& second) {
    const Time firstRTime = first.template get<Rtime>().get();
    const Time secondRTime = second.template get<Rtime>().get();

    return Duration{subtractTimes(secondRTime, firstRTime)};
  }

  template <typename BlockType>
  Duration calculateDuration(const BlockType& block,
                             Time channelFormatDuration) {
    return Duration{subtractTimes(channelFormatDuration,
                                  block.template get<Rtime>().get())};
  }

  template <typename BlockType>
  void setDurationIfNotEqual(BlockType& block, const Duration& newDuration) {
    if (!block.template has<Duration>() ||
        !timesEqual(block.template get<Duration>().get(), newDuration.get()))
      block.set(newDuration);
  }

  template <typename BlockType>
  void updateBlockFormatDurationWithType(AudioChannelFormat* channel,
                                         Time channelFormatDuration) {
    auto current = begin(channel->getElements<BlockType>());
    // no blocks
    if (current == end(channel->getElements<BlockType>()))
      throw error::detail::formatElementRuntimeError(
          channel->get<AudioChannelFormatId>(),
          "AudioChannelFormat has no audioBlockFormats");

    auto next = current;
    std::advance(next, 1);
    while (next != end(channel->getElements<BlockType>())) {
      // a single time can have multiple representations, so if the current
      // time is correct it should be kept
      setDurationIfNotEqual(*current, calculateDuration(*current, *next));

      std::advance(current, 1);
      std::advance(next, 1);
    }
    // here, current is always the last block

    setDurationIfNotEqual(*current,
                          calculateDuration(*current, channelFormatDuration));
  }

  void updateBlockFormatDuration(AudioChannelFormat* channel,
                                 Time channelFormatDuration) {
    auto typedefinition = channel->get<TypeDescriptor>();
    if (typedefinition == TypeDefinition::DIRECT_SPEAKERS) {
      updateBlockFormatDurationWithType<AudioBlockFormatDirectSpeakers>(
          channel, channelFormatDuration);
    } else if (typedefinition == TypeDefinition::MATRIX) {
      updateBlockFormatDurationWithType<AudioBlockFormatMatrix>(
          channel, channelFormatDuration);
    } else if (typedefinition == TypeDefinition::OBJECTS) {
      updateBlockFormatDurationWithType<AudioBlockFormatObjects>(
          channel, channelFormatDuration);
    } else if (typedefinition == TypeDefinition::HOA) {
      updateBlockFormatDurationWithType<AudioBlockFormatHoa>(
          channel, channelFormatDuration);
    } else if (typedefinition == TypeDefinition::BINAURAL) {
      updateBlockFormatDurationWithType<AudioBlockFormatBinaural>(
          channel, channelFormatDuration);
    } else {
      assert(0);
      throw std::logic_error("Unhandled type definition found");
    }
  }

  bool isPresentWithDifferentValue(
      AudioChannelFormatId channel, Time duration,
      const std::map<AudioChannelFormatId, Time>& durations) {
    return durations.count(channel) > 0 &&
           !timesEqual(durations.at(channel), duration);
  }

  std::map<AudioChannelFormatId, Time> calculateDurationOfChannels(
      std::shared_ptr<const AudioProgramme> programme,
      boost::optional<Time> fileLength) {
    auto programmeDuration = durationOfProgramme(programme.get(), fileLength);
    RouteTracer tracer;
    auto result = tracer.run(programme);
    std::map<AudioChannelFormatId, Time> durations;
    for (const auto& route : result) {
      auto duration = durationOfChannel(route, programmeDuration);
      auto channel =
          route.getLastOf<AudioChannelFormat>()->get<AudioChannelFormatId>();
      if (isPresentWithDifferentValue(channel, duration, durations)) {
        throw error::detail::formatElementRuntimeError(
            channel,
            "AudioChannelFormat with different effective durations detected");
      }
      durations.insert(std::make_pair(channel, duration));
    }
    return durations;
  }

  void updateBlockFormatDurationsImpl(std::shared_ptr<Document> document,
                                      boost::optional<Time> fileLength) {
    std::map<AudioChannelFormatId, Time> durations;
    for (auto& programme : document->template getElements<AudioProgramme>()) {
      auto durationsOfProgramme =
          calculateDurationOfChannels(programme, fileLength);
      for (const auto& duration : durationsOfProgramme) {
        if (isPresentWithDifferentValue(duration.first, duration.second,
                                        durations)) {
          throw error::detail::formatElementRuntimeError(
              duration.first,
              "AudioChannelFormat referenced by multiple programmes with "
              "different effective durations detected");
        }
        durations.insert(duration);
      }
    }

    for (auto& entry : durations) {
      auto channel = document->lookup(entry.first);
      auto duration = entry.second;
      updateBlockFormatDuration(channel.get(), duration);
    }
  }

  void updateBlockFormatDurations(std::shared_ptr<Document> document,
                                  const Time& fileLength) {
    updateBlockFormatDurationsImpl(document, fileLength);
  }

  void updateBlockFormatDurations(std::shared_ptr<Document> document) {
    if (document->getElements<AudioProgramme>().empty()) {
      throw error::AdmGenericRuntimeError(
          "No audio programme present, cannot guess length");
    }
    updateBlockFormatDurationsImpl(document, boost::none);
  }

}  // namespace adm
