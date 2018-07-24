#include <adm/utilities/block_duration_assignment.hpp>
#include <adm/helper/get_optional_property.hpp>
#include <adm/document.hpp>
#include <adm/route_tracer.hpp>
#include <memory>

namespace adm {
  std::chrono::nanoseconds calculateDurationOfChannel(
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
      throw std::logic_error("Unhandled type definition found");
    }
  }

  void updateBlockFormatDurations(std::shared_ptr<Document> document,
                                  std::chrono::nanoseconds fileLength) {
    RouteTracer tracer;
    auto result =
        tracer.run(document->template getElements<AudioProgramme>()[0]);
    std::map<AudioChannelFormatId, std::chrono::nanoseconds> durations;
    for (const auto& route : result) {
      auto duration = calculateDurationOfChannel(route, fileLength);
      auto channel =
          route.getLastOf<AudioChannelFormat>()->get<AudioChannelFormatId>();
      if (durations.count(channel) > 0 && durations.at(channel) != duration) {
        throw std::runtime_error(
            "AudioChannelFormat cannot have different effective durations");
      }
      durations.insert(std::make_pair(channel, duration));
    }
    for (auto& entry : durations) {
      auto channel = document->lookup(entry.first);
      auto duration = entry.second;
      updateBlockFormatDuration(channel.get(), duration);
    }
  }

  void updateBlockFormatDurations(std::shared_ptr<Document> document) {
    auto programme = document->getElements<AudioProgramme>()[0];

    if (programme->has<End>()) {
      updateBlockFormatDurations(document, programme->get<End>().get() -
                                               programme->get<Start>().get());
    }
  }

}  // namespace adm
