#pragma once

#include "adm/document.hpp"
#include "adm/frame.hpp"
#include <memory>
#include <map>
#include "adm/libadm_export.h"

namespace adm {

  class Combiner {
   public:
    LIBADM_EXPORT Combiner();

    LIBADM_EXPORT void push(std::shared_ptr<const Frame> frame);

    LIBADM_EXPORT std::shared_ptr<Document> getDocument();
    LIBADM_EXPORT TransportTrackFormat getTransportTrackFormat();

   private:
    void updateDocument(std::shared_ptr<const Frame> frame);
    void updateTransportTrackFormats(std::shared_ptr<const Frame> frame);
    void cumulateAudioTrack(AudioTrack audioTrack);

    std::shared_ptr<Document> document_;
    TransportTrackFormat transportTrackFormat_;
    bool firstFrame_;
    std::map<AudioChannelFormatId, AudioBlockFormatId> lastBlockFormatIds_;

    template <typename BlockFormat>
    void addBlockFormats(std::shared_ptr<const AudioChannelFormat> src,
                         std::shared_ptr<AudioChannelFormat> dest);

    template <typename Src, typename TargetId>
    void resolveReferences(std::map<Src, std::vector<TargetId>> map) {
      for (auto entry : map) {
        for (auto id : entry.second) {
          entry.first->addReference(document_->lookup(id));
        }
      }
    }

    template <typename Src, typename Target>
    void resolveReference(std::map<Src, Target> map) {
      for (auto entry : map) {
        entry.first->setReference(document_->lookup(entry.second));
      }
    }
  };

  // ---- Implementation ---- //

  template <typename BlockFormat>
  void Combiner::addBlockFormats(std::shared_ptr<const AudioChannelFormat> src,
                                 std::shared_ptr<AudioChannelFormat> dest) {
    auto blockFormatsSrc = src->template getElements<BlockFormat>();
    auto channelFormatIdDest = dest->get<AudioChannelFormatId>();
    AudioBlockFormatId blockFormatId;
    for (auto blockFormat : blockFormatsSrc) {
      blockFormatId = blockFormat.template get<AudioBlockFormatId>();
      if (lastBlockFormatIds_.at(channelFormatIdDest) < blockFormatId) {
        dest->add(blockFormat);
        lastBlockFormatIds_.at(channelFormatIdDest) = blockFormatId;
      }
    }
  }

}  // namespace adm
