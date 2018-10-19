#include <adm/utilities/block_duration_assignment.hpp>
#include <adm/helper/get_optional_property.hpp>
#include <adm/document.hpp>
#include <adm/errors.hpp>
#include <adm/route_tracer.hpp>
#include <memory>
#include <map>
#include <stdexcept>

namespace adm {

  std::chrono::nanoseconds durationOfProgramme(
      const AudioProgramme* programme,
      boost::optional<std::chrono::nanoseconds> fileLength) {
    if (programme->has<End>()) {
      auto duration =
          programme->get<End>().get() - programme->get<Start>().get();
      // if a file length is given AND a programme end is set, both durations
      // must match
      if (fileLength && fileLength.get() != duration) {
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

  std::chrono::nanoseconds durationOfChannel(
      const Route& route, std::chrono::nanoseconds fileLength) {
    assert(
        isVariantType<std::shared_ptr<const AudioChannelFormat>>(route.back()));
    auto lastObject = route.getLastOf<AudioObject>();
    auto effectiveDuration = getPropertyOr(lastObject, Duration(fileLength));
    return effectiveDuration.get();
  }

  template <typename BlockType>
  Duration calculateDuration(const BlockType& first, const BlockType& second) {
    return Duration{second.template get<Rtime>().get() -
                    first.template get<Rtime>().get()};
  }

  template <typename BlockType>
  Duration calculateDuration(const BlockType& block,
                             std::chrono::nanoseconds channelFormatDuration) {
    return Duration{channelFormatDuration - block.template get<Rtime>().get()};
  }

  template <typename BlockType>
  void updateBlockFormatDurationWithType(
      AudioChannelFormat* channel,
      std::chrono::nanoseconds channelFormatDuration) {
    auto current = begin(channel->getElements<BlockType>());
    auto next = current;
    std::advance(next, 1);
    while (next != end(channel->getElements<BlockType>())) {
      current->set(calculateDuration(*current, *next));
      std::advance(current, 1);
      std::advance(next, 1);
    }
    if (current != end(channel->getElements<BlockType>())) {
      current->set(calculateDuration(*current, channelFormatDuration));
    }
  }

  void updateBlockFormatDuration(
      AudioChannelFormat* channel,
      std::chrono::nanoseconds channelFormatDuration) {
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
      AudioChannelFormatId channel, std::chrono::nanoseconds duration,
      const std::map<AudioChannelFormatId, std::chrono::nanoseconds>&
          durations) {
    return durations.count(channel) > 0 && durations.at(channel) != duration;
  }

  std::map<AudioChannelFormatId, std::chrono::nanoseconds>
  calculateDurationOfChannels(
      std::shared_ptr<const AudioProgramme> programme,
      boost::optional<std::chrono::nanoseconds> fileLength) {
    auto programmeDuration = durationOfProgramme(programme.get(), fileLength);
    RouteTracer tracer;
    auto result = tracer.run(programme);
    std::map<AudioChannelFormatId, std::chrono::nanoseconds> durations;
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

  void updateBlockFormatDurationsImpl(
      std::shared_ptr<Document> document,
      boost::optional<std::chrono::nanoseconds> fileLength) {
    std::map<AudioChannelFormatId, std::chrono::nanoseconds> durations;
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
                                  std::chrono::nanoseconds fileLength) {
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
