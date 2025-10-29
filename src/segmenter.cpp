
#include "adm/segmenter.hpp"
#include "adm/route_tracer.hpp"
#include "adm/route.hpp"
#include "adm/elements.hpp"
#include "adm/private/copy.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/block_duration_assignment.hpp"
#include <algorithm>
#include <set>

#undef NDEBUG
#include <assert.h>

namespace adm {

  Segmenter::Segmenter(std::shared_ptr<Document> document, boost::optional<Time> fileLength)
      : document_(document) {

    // Sort elements
    for (auto channelFormat : document_->getElements<AudioChannelFormat>()) {
      sortBlockFormats<AudioBlockFormatDirectSpeakers>(channelFormat);
      sortBlockFormats<AudioBlockFormatMatrix>(channelFormat);
      sortBlockFormats<AudioBlockFormatObjects>(channelFormat);
      sortBlockFormats<AudioBlockFormatHoa>(channelFormat);
      sortBlockFormats<AudioBlockFormatBinaural>(channelFormat);
    }

    detail::GenericRouteTracer<adm::Route, CustomStrategy> tracer;
    //RouteTracer tracer;
    for (auto audioProgramme : document_->getElements<AudioProgramme>()) {
      auto routes = tracer.run(audioProgramme);
      for (auto route : routes) {
        auto programme = route.getFirstOf<AudioProgramme>();
        auto content = route.getFirstOf<AudioContent>();
        auto objects = route.getAllOf<AudioObject>();
        auto packFormats = route.getAllOf<AudioPackFormat>();
        auto channelFormat = route.getLastOf<AudioChannelFormat>();
        auto streamFormat = route.getLastOf<AudioStreamFormat>();
        auto trackFormat = route.getLastOf<AudioTrackFormat>();
        auto trackUid = route.getLastOf<AudioTrackUid>();

        std::chrono::nanoseconds objectStart = std::chrono::nanoseconds{0};
        boost::optional<std::chrono::nanoseconds> objectEnd;
        objectEnd = durationOfProgramme(programme.get(), fileLength).asNanoseconds();

        for (auto object : objects) {
          if (object->get<Start>().get().asNanoseconds() >= objectStart) {
            objectStart = object->get<Start>().get().asNanoseconds();
          }
          if (object->has<Duration>()) {
            if (objectEnd) {
              if (objectStart + object->get<Duration>().get().asNanoseconds() <= objectEnd) {
                objectEnd = objectStart + object->get<Duration>().get().asNanoseconds();
              }
            } else {
              objectEnd = objectStart + object->get<Duration>().get().asNanoseconds();
            }
          }
        }

        detail::SegmenterItem newItem(programme, content, objects,
                                      packFormats, channelFormat, streamFormat,
                                      trackFormat, trackUid, objectStart, objectEnd);
        segmenterItems_.push_back(newItem);
      }
    }
  }


  void Segmenter::buildFrame(SegmentStart segStart,
                             SegmentDuration segDuration,
                             unsigned int frameNum) {
    // create baseFrame
    baseFrame_ = Document::create();

    if (baseFrame_->getElements<AudioChannelFormat>().size() > 0) {
      for (auto channelFormat : baseFrame_->getElements<AudioChannelFormat>()) {
        channelFormat->clearAudioBlockFormats();
      }
    }

    FrameIndex fr_index = FrameIndex(frameNum + 1);
    auto frameFormatId = FrameFormatId(fr_index); 
    auto frameFormat = FrameFormat{
      frameFormatId, Start(segStart.get()),
      Duration(segDuration.get()), FrameType::FULL};
    frameFormat.set(TimeReference::LOCAL);
    auto frameHeader = FrameHeader{frameFormat};

    frameHeader_ = std::make_shared<FrameHeader>(frameHeader);

    for (auto &item : segmenterItems_) {
      item.use = false;
      uint32_t numBlocks = 0;

      std::shared_ptr<AudioTrackUid> trackUidDest = nullptr;
      if (item.trackUid != nullptr) {
        trackUidDest = item.trackUid->copy();
      }
      std::shared_ptr<AudioTrackFormat> trackFormatDest = nullptr;
      if (item.trackFormat != nullptr) {
        trackFormatDest = item.trackFormat->copy();
      }
      std::shared_ptr<AudioStreamFormat> streamFormatDest = nullptr;
      if (item.streamFormat != nullptr) {
        streamFormatDest = item.streamFormat->copy();
      }

      std::shared_ptr<AudioChannelFormat> channelFormatDest = item.channelFormat->copy();
      channelFormatDest->clearAudioBlockFormats();  // Clear blocks, as they will be populated later

      std::vector<std::shared_ptr<AudioPackFormat>> packFormatDests;
      for (auto &packFormat : item.packFormats) {
        packFormatDests.push_back(packFormat->copy());
      }
      std::vector<std::shared_ptr<AudioObject>> objectDests;
      for (auto &object : item.objects) {
        objectDests.push_back(object->copy());
      }
      std::shared_ptr<AudioContent> contentDest = item.content->copy();
      std::shared_ptr<AudioProgramme> programmeDest = item.programme->copy();

      if (channelFormatDest->get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS) {
        numBlocks = addItemToChannelFormat<AudioBlockFormatDirectSpeakers>(
            item, segStart, segDuration, channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::MATRIX) {
        numBlocks = addItemToChannelFormat<AudioBlockFormatMatrix>(
            item, segStart, segDuration, channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::OBJECTS) {
        numBlocks = addItemToChannelFormat<AudioBlockFormatObjects>(
            item, segStart, segDuration, channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::HOA) {
        numBlocks = addItemToChannelFormat<AudioBlockFormatHoa>(item, segStart, segDuration,
                                                    channelFormatDest);
      } else if (channelFormatDest->get<TypeDescriptor>() ==
                 TypeDefinition::BINAURAL) {
        numBlocks = addItemToChannelFormat<AudioBlockFormatBinaural>(item, segStart, segDuration,
                                                    channelFormatDest);
      }

      if (numBlocks > 0) {
        item.use = true;
        if (trackUidDest != nullptr) {
          if (baseFrame_->lookup(trackUidDest->get<AudioTrackUidId>()) == nullptr) {
            baseFrame_->add(trackUidDest);
          }
        }
        if (trackFormatDest != nullptr) {
          if (baseFrame_->lookup(trackFormatDest->get<AudioTrackFormatId>()) == nullptr) {
            baseFrame_->add(trackFormatDest);
          }
        }
        if (streamFormatDest != nullptr) {
          if (baseFrame_->lookup(streamFormatDest->get<AudioStreamFormatId>()) == nullptr) {
            baseFrame_->add(streamFormatDest);
          }
        }
        if (baseFrame_->lookup(channelFormatDest->get<AudioChannelFormatId>()) == nullptr) {
          baseFrame_->add(channelFormatDest);
        }
        for (auto &packFormatDest : packFormatDests) {
          if (baseFrame_->lookup(packFormatDest->get<AudioPackFormatId>()) == nullptr) {
            baseFrame_->add(packFormatDest);
          }
        }
        for (auto &objectDest : objectDests) {
          if (baseFrame_->lookup(objectDest->get<AudioObjectId>()) == nullptr) {
            baseFrame_->add(objectDest);
          }
        }
      }
      // Always include audioContent and audioProgramme even if there's no audioObjects at this time
      if (baseFrame_->lookup(contentDest->get<AudioContentId>()) == nullptr) {
        baseFrame_->add(contentDest);
      }
      if (baseFrame_->lookup(programmeDest->get<AudioProgrammeId>()) == nullptr) {
        baseFrame_->add(programmeDest);
      }
    }

    fixReferences();
  }


  void Segmenter::fixReferences() {
    for (auto item : segmenterItems_) {
      auto programme = baseFrame_->lookup(item.programme->get<AudioProgrammeId>());
      auto content = baseFrame_->lookup(item.content->get<AudioContentId>());
      programme->addReference(content);
      if (item.use) {
        std::vector<std::shared_ptr<AudioObject>> objects;
        for (auto object : item.objects) {
          objects.push_back(baseFrame_->lookup(object->get<AudioObjectId>()));
        }
        std::vector<std::shared_ptr<AudioPackFormat>> packFormats;
        for (auto packFormat : item.packFormats) {
          packFormats.push_back(baseFrame_->lookup(packFormat->get<AudioPackFormatId>()));
        }
        auto channelFormat = baseFrame_->lookup(item.channelFormat->get<AudioChannelFormatId>());
        std::shared_ptr<AudioStreamFormat> streamFormat = nullptr;
        if (item.streamFormat != nullptr) {
          streamFormat = baseFrame_->lookup(item.streamFormat->get<AudioStreamFormatId>());
        }
        std::shared_ptr<AudioTrackFormat> trackFormat = nullptr;
        if (item.trackFormat != nullptr) {
          trackFormat = baseFrame_->lookup(item.trackFormat->get<AudioTrackFormatId>());
        }
        std::shared_ptr<AudioTrackUid> trackUid = nullptr;
        if (item.trackUid != nullptr) {
          trackUid = baseFrame_->lookup(item.trackUid->get<AudioTrackUidId>());
        }

        content->addReference(objects[0]);
        for (uint32_t i = 0; i < objects.size() - 1; i++) {
          objects[i]->addReference(objects[i + 1]);
        }
        if (packFormats.size() > 0) {
          objects.back()->addReference(packFormats[0]);
        }
        if (trackUid) objects.back()->addReference(trackUid);
        if (packFormats.size() > 0) {
          packFormats.back()->addReference(channelFormat);
        }
        if (streamFormat && trackFormat) streamFormat->addReference(trackFormat);
        if (streamFormat) streamFormat->setReference(channelFormat);
        if (trackUid && trackFormat) trackUid->setReference(trackFormat);
        if (trackUid && channelFormat) trackUid->setReference(channelFormat);
        if (packFormats.size() > 0) {
          if (trackUid && packFormats[0]) trackUid->setReference(packFormats[0]);
        }
      }
    }
  }

  TransportTrackFormat Segmenter::generateTransportTrackFormat(
                                   TrackUidList trackUidList,
                                   SegmentStart segStart,
                                   SegmentDuration segDuration) {
    auto transportTrackFormat = TransportTrackFormat(TransportId(TransportIdValue(1)));
    transportTrackFormat.set(TransportId(TransportIdValue(1)));  // Assuming only one set

    std::vector<AudioTrack> audioTracks; // Vector of audioTracks to build up

    for (int i = 0; i < trackUidList.trackUid.size(); i++) {
      auto track_id = TrackId(trackUidList.trackUid[i].trackIndex);
      bool exists = false;
      for (auto &audioTrack : audioTracks) {
        if (audioTrack.get<TrackId>() == track_id) {
          auto audioTrackUidId = parseAudioTrackUidId(trackUidList.trackUid[i].uid);
          bool present = checkAudioObjectTimes(audioTrackUidId, segStart, segDuration);
          if (present) {
            audioTrack.add(audioTrackUidId);
          }
          exists = true;
        }
      }
      if (!exists) {
        AudioTrack audioTrack(track_id);
        auto audioTrackUidId = parseAudioTrackUidId(trackUidList.trackUid[i].uid);
        bool present = checkAudioObjectTimes(audioTrackUidId, segStart, segDuration);
        if (present) {
          audioTrack.add(audioTrackUidId);
          audioTracks.push_back(audioTrack);
        }
      }
    }
    for (auto audioTrack : audioTracks) {
      transportTrackFormat.add(audioTrack);
    }

    frameHeader_->add(transportTrackFormat);

    return transportTrackFormat;
  }


  bool Segmenter::checkAudioObjectTimes(AudioTrackUidId audioTrackUidId_ref,
                                        SegmentStart segStart,
                                        SegmentDuration segDuration) {
    // Checks if the audioTrackUids in audioObjects are within the time for the segment
    bool present = true;
    for (auto audioObject : document_->getElements<AudioObject>()) {
      for (auto& audioTrackUid : audioObject->getReferences<AudioTrackUid>()) {
        if (audioTrackUidId_ref == audioTrackUid->get<AudioTrackUidId>()) {
          if (audioObject->has<Start>()) {
            auto start = audioObject->get<Start>();
            if (start.get().asNanoseconds() > segStart.get() + segDuration.get()) {
              present = false;
            }
            if (audioObject->has<Duration>()) {
              auto duration = audioObject->get<Duration>();
              if (start.get().asNanoseconds() + duration.get().asNanoseconds() <= segStart.get()) {
                present = false;
              }
            }
          }
        }
      }
    }
    return present;
  }

}  // namespace adm
