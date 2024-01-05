#include "adm/detail/id_assigner.hpp"
#include "adm/document.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/elements.hpp"

namespace adm {
  namespace detail {
    namespace {
      struct DefaultFilter {
        template <typename T>
        bool operator()(T const&) const {
          return true;
        }
      };
      // original recursive lookup method was O(N^2), this is O(NlogN)
      // N for filter by type / value
      // NlogN for sort
      // logN for lower_bound
      // N for adjacent_find
      // if using pre-sorted vectors would be O(N)
      template <typename ElementT, typename CounterT,
                typename FilterPredicate = DefaultFilter>
      CounterT nextCounter(adm::Document const& doc, CounterT preferredCounter,
                           FilterPredicate predicate = FilterPredicate{}) {
        auto elements = doc.getElements<ElementT>();
        // Filter ids by predicate, collect counter values in container
        std::vector<typename CounterT::value_type> counters;
        counters.reserve(elements.size());
        for (auto const& el : elements) {
          auto id = el->template get<typename ElementT::id_type>();
          if (predicate(id)) {
            counters.push_back(id.template get<CounterT>().get());
          }
        }
        // sort counters so we can use fast algorithms
        std::sort(counters.begin(), counters.end());
        // find the position where all elements are >= preferred value
        auto lowerBound = std::lower_bound(counters.begin(), counters.end(),
                                           preferredCounter.get());
        // if this is the end of the vector or lower bound is not equal to preferred, preferred is free so use it
        if (lowerBound == counters.end() ||
            *lowerBound != preferredCounter.get()) {
          return preferredCounter;
        }

        // starting from the preferred value, find the next free value by looking for gaps in sequence
        auto it = std::adjacent_find(
            lowerBound, counters.end(),
            [](auto lhs, auto rhs) { return lhs + 1 != rhs; });
        // if no gaps use next available value
        if (it == counters.end()) {
          return CounterT(counters.back() + 1);
        }
        // if gap, use that
        return CounterT((*it) + 1);
      }
    }  // namespace

    const Document& IdAssigner::document() const { return *document_; }

    void IdAssigner::document(Document* document) { document_ = document; }

    AudioProgrammeId IdAssigner::assignId(AudioProgramme& programme) {
      if (isCommonDefinitionsId(programme.get<AudioProgrammeId>())) {
        return programme.get<AudioProgrammeId>();
      }
      AudioProgrammeIdValue idValue(0x1001u);
      if (!isUndefined(programme.get<AudioProgrammeId>())) {
        idValue =
            programme.get<AudioProgrammeId>().get<AudioProgrammeIdValue>();
      }
      idValue = nextCounter<AudioProgramme>(document(), idValue);
      auto id = AudioProgrammeId(idValue);
      programme.set(id);
      return id;
    }

    AudioContentId IdAssigner::assignId(AudioContent& content) {
      if (isCommonDefinitionsId(content.get<AudioContentId>())) {
        return content.get<AudioContentId>();
      }
      AudioContentIdValue idValue(0x1001u);
      if (!isUndefined(content.get<AudioContentId>())) {
        idValue = content.get<AudioContentId>().get<AudioContentIdValue>();
      }
      idValue = nextCounter<AudioContent>(document(), idValue);
      auto id = AudioContentId(idValue);
      content.set(id);
      return id;
    }

    AudioObjectId IdAssigner::assignId(AudioObject& object) {
      if (isCommonDefinitionsId(object.get<AudioObjectId>())) {
        return object.get<AudioObjectId>();
      }
      AudioObjectIdValue idValue(0x1001u);
      if (!isUndefined(object.get<AudioObjectId>())) {
        idValue = object.get<AudioObjectId>().get<AudioObjectIdValue>();
      }
      idValue = nextCounter<AudioObject>(document(), idValue);
      auto id = AudioObjectId(idValue);
      object.set(id);
      return id;
    }

    AudioPackFormatId IdAssigner::assignId(AudioPackFormat& packFormat) {
      if (isCommonDefinitionsId(packFormat.get<AudioPackFormatId>())) {
        return packFormat.get<AudioPackFormatId>();
      }
      AudioPackFormatIdValue idValue(0x1001u);
      auto typeDescriptor = packFormat.get<TypeDescriptor>();
      if (!isUndefined(packFormat.get<AudioPackFormatId>())) {
        idValue =
            packFormat.get<AudioPackFormatId>().get<AudioPackFormatIdValue>();
      }
      idValue = nextCounter<AudioPackFormat>(
          document(), idValue, [typeDescriptor](AudioPackFormatId const& id) {
            return id.get<TypeDescriptor>() == typeDescriptor;
          });
      auto id = AudioPackFormatId(typeDescriptor, idValue);
      packFormat.set(id);
      return id;
    }

    AudioChannelFormatId IdAssigner::assignId(
        AudioChannelFormat& channelFormat) {
      if (isCommonDefinitionsId(channelFormat.get<AudioChannelFormatId>())) {
        return channelFormat.get<AudioChannelFormatId>();
      }
      AudioChannelFormatIdValue idValue(0x1001u);
      auto typeDescriptor = channelFormat.get<TypeDescriptor>();
      if (!isUndefined(channelFormat.get<AudioChannelFormatId>())) {
        idValue = channelFormat.get<AudioChannelFormatId>()
                      .get<AudioChannelFormatIdValue>();
      }
      idValue = nextCounter<AudioChannelFormat>(
          document(), idValue,
          [typeDescriptor](AudioChannelFormatId const& id) {
            return id.get<TypeDescriptor>() == typeDescriptor;
          });
      auto id = AudioChannelFormatId(typeDescriptor, idValue);
      channelFormat.set(id);
      return id;
    }

    AudioStreamFormatId IdAssigner::assignId(AudioStreamFormat& streamFormat) {
      if (isCommonDefinitionsId(streamFormat.get<AudioStreamFormatId>())) {
        return streamFormat.get<AudioStreamFormatId>();
      }
      auto typeDescriptor = TypeDefinition::UNDEFINED;
      AudioStreamFormatIdValue idValue(0x1001u);
      if (!isUndefined(streamFormat.get<AudioStreamFormatId>())) {
        typeDescriptor =
            streamFormat.get<AudioStreamFormatId>().get<TypeDescriptor>();
        idValue = streamFormat.get<AudioStreamFormatId>()
                      .get<AudioStreamFormatIdValue>();
      } else {
        auto channelFormat = streamFormat.getReference<AudioChannelFormat>();
        auto packFormat = streamFormat.getReference<AudioPackFormat>();
        if (channelFormat) {
          typeDescriptor = channelFormat->get<TypeDescriptor>();
        } else if (packFormat) {
          typeDescriptor = packFormat->get<TypeDescriptor>();
        }
      }
      idValue = nextCounter<AudioStreamFormat>(
          document(), idValue, [typeDescriptor](AudioStreamFormatId const& id) {
            return id.get<TypeDescriptor>() == typeDescriptor;
          });
      auto id = AudioStreamFormatId(typeDescriptor, idValue);
      streamFormat.set(id);
      return id;
    }

    AudioTrackFormatId IdAssigner::assignId(AudioTrackFormat& trackFormat) {
      if (isCommonDefinitionsId(trackFormat.get<AudioTrackFormatId>())) {
        return trackFormat.get<AudioTrackFormatId>();
      }
      auto typeDescriptor = TypeDefinition::UNDEFINED;
      AudioTrackFormatIdValue idValue(0x1001u);
      AudioTrackFormatIdCounter idCounter(1);
      if (!isUndefined(trackFormat.get<AudioTrackFormatId>())) {
        idValue = trackFormat.get<AudioTrackFormatId>()
                      .get<AudioTrackFormatIdValue>();
        idCounter = trackFormat.get<AudioTrackFormatId>()
                        .get<AudioTrackFormatIdCounter>();
        typeDescriptor =
            trackFormat.get<AudioTrackFormatId>().get<TypeDescriptor>();
      } else {
        auto streamFormat = trackFormat.getReference<AudioStreamFormat>();
        if (streamFormat) {
          auto streamFormatId = streamFormat->get<AudioStreamFormatId>();
          idValue = AudioTrackFormatIdValue(
              streamFormatId.get<AudioStreamFormatIdValue>().get());
          typeDescriptor = streamFormatId.get<TypeDescriptor>();
        }
      }
      idCounter = nextCounter<AudioTrackFormat>(
          document(), idCounter,
          [&typeDescriptor, &idValue](AudioTrackFormatId const& id) {
            return id.get<TypeDescriptor>() == typeDescriptor &&
                   id.get<AudioTrackFormatIdValue>() == idValue;
          });
      auto id = AudioTrackFormatId(typeDescriptor, idValue, idCounter);
      trackFormat.set(id);
      return id;
    }

    AudioTrackUidId IdAssigner::assignId(AudioTrackUid& trackUid) {
      if (isCommonDefinitionsId(trackUid.get<AudioTrackUidId>()) ||
          trackUid.isSilent()) {
        return trackUid.get<AudioTrackUidId>();
      }
      AudioTrackUidIdValue idValue(0x0001u);
      if (!isUndefined(trackUid.get<AudioTrackUidId>())) {
        idValue = trackUid.get<AudioTrackUidId>().get<AudioTrackUidIdValue>();
      }
      idValue = nextCounter<AudioTrackUid>(document(), idValue);
      auto id = AudioTrackUidId(idValue);
      trackUid.set(id);
      return id;
    }

  }  // namespace detail
}  // namespace adm
