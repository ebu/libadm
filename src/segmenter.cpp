
#include "adm/segmenter.hpp"
#include "adm/route_tracer.hpp"
#include "adm/route.hpp"
#include "adm/elements.hpp"
#include "adm/private/copy.hpp"
#include "adm/utilities/copy.hpp"
#include <algorithm>
#include <set>

#undef NDEBUG
#include <assert.h>

namespace adm {

  Segmenter::Segmenter(std::shared_ptr<Document> document)
      : document_(document) {
    // create baseFrame
    baseFrame_ = Frame::create(FrameStart(std::chrono::nanoseconds(0)),
                               FrameDuration(std::chrono::nanoseconds(0)),
                               FrameType("full"));

    for (auto channelFormat : document_->getElements<AudioChannelFormat>()) {
      sortBlockFormats<AudioBlockFormatDirectSpeakers>(channelFormat);
      sortBlockFormats<AudioBlockFormatMatrix>(channelFormat);
      sortBlockFormats<AudioBlockFormatObjects>(channelFormat);
      sortBlockFormats<AudioBlockFormatHoa>(channelFormat);
      sortBlockFormats<AudioBlockFormatBinaural>(channelFormat);
    }

    deepCopyTo(document_, baseFrame_);

    for (auto channelFormat : baseFrame_->getElements<AudioChannelFormat>()) {
      channelFormat->clearAudioBlockFormats();
    }

    RouteTracer tracer;
    for (auto audioProgramme : document_->getElements<AudioProgramme>()) {
      auto routes = tracer.run(audioProgramme);
      for (auto route : routes) {
        auto channelFormat = route.getLastOf<AudioChannelFormat>();
        auto programme = route.getFirstOf<AudioProgramme>();
        auto object = route.getLastOf<AudioObject>();
        std::chrono::nanoseconds objectStart = object->get<Start>().get();
        boost::optional<std::chrono::nanoseconds> objectEnd;
        if (object->has<Duration>()) {
          objectEnd = objectStart + object->get<Duration>().get();
        } else if (programme->has<End>()) {
          objectEnd =
              programme->get<End>().get() - programme->get<Start>().get();
        }
        detail::SegmenterItem newItem(channelFormat, objectStart, objectEnd);
        auto it = std::find_if(
            segmenterItems_.begin(), segmenterItems_.end(),
            [&newItem](const detail::SegmenterItem& item) {
              return newItem.channelFormat == item.channelFormat &&
                     newItem.start == item.start && newItem.end == item.end;
            });
        if (it == segmenterItems_.end()) {
          segmenterItems_.push_back(newItem);
        }
      }
    }
  }

  void Segmenter::addTransportTrackFormat(
      const TransportTrackFormat& transportTrackFormat) {
    baseFrame_->frameHeader().add(transportTrackFormat);
  }

  std::shared_ptr<Frame> Segmenter::getFrame(SegmentStart segStart,
                                             SegmentDuration segDuration) {
    auto admFrame = baseFrame_->deepCopy();

    auto idValue = admFrame->frameHeader()
                       .frameFormat()
                       .get<FrameFormatId>()
                       .get<FrameFormatIdValue>();
    admFrame->frameHeader().frameFormat().set(FrameFormatId(++idValue));
    admFrame->frameHeader().frameFormat().set(FrameStart(segStart.get()));
    admFrame->frameHeader().frameFormat().set(FrameDuration(segDuration.get()));

    for (auto item : segmenterItems_) {
      auto channelFormatDest =
          admFrame->lookup(item.channelFormat->get<AudioChannelFormatId>());
      if (channelFormatDest->get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS) {
        addItemToChannelFormat<AudioBlockFormatDirectSpeakers>(
            item, segStart, segDuration, channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::MATRIX) {
        addItemToChannelFormat<AudioBlockFormatMatrix>(
            item, segStart, segDuration, channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::OBJECTS) {
        addItemToChannelFormat<AudioBlockFormatObjects>(
            item, segStart, segDuration, channelFormatDest);

      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::HOA) {
        addItemToChannelFormat<AudioBlockFormatHoa>(item, segStart, segDuration,
                                                    channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::BINAURAL) {
        addItemToChannelFormat<AudioBlockFormatHoa>(item, segStart, segDuration,
                                                    channelFormatDest);
      }
    }

    return admFrame;
  }

}  // namespace adm
