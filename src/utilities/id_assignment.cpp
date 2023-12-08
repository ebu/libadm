#include "adm/utilities/id_assignment.hpp"

namespace adm {
  namespace {
    // A struct for keeping track of the next available ID value for all element types
    // if reassigning all ids (so no need to lookup)
    struct IdValues {
      struct ChannelStreamOrTrackFormat {};
      // start counting id values at 0x1001 for all Ids except
      template <typename T>
      struct NextIndex {
        uint32_t value{0x1001u};
      };
      // AudioTrackUID, which should start from 1
      template <>
      struct NextIndex<AudioTrackUid> {
        uint32_t value{0x1u};
      };
      // Track an index for each independent element type without
      // a TypeDescriptor property
      std::tuple<NextIndex<AudioProgramme>, NextIndex<AudioContent>,
                 NextIndex<AudioObject>, NextIndex<AudioTrackUid>>
          values;

      constexpr static std::size_t TYPEDESCRIPTOR_COUNT{6};
      // for elements with TypeDescriptor dependent ids, use an array indexed
      // on the value of the TypeDescriptor
      template <typename T>
      using TypedIndices = std::array<NextIndex<T>, TYPEDESCRIPTOR_COUNT>;

      // linked Channel, Stream and Track formats share the index part of id
      std::tuple<TypedIndices<ChannelStreamOrTrackFormat>,
                 TypedIndices<AudioPackFormat>>  // pack format is independent
          typedValues;

      template <typename T>
      using IdValue = typename T::id_type::value_type;

      template <typename T>
      IdValue<T> next() {
        return typename T::id_type::value_type{
            std::get<NextIndex<T>>(values).value++};
      }

      // get the next Channel/Stream/Track linked id value for this TypeDescriptor
      template <typename T>
      IdValue<T> next(TypeDescriptor descriptor) {
        return typename T::id_type::value_type{
            std::get<TypedIndices<ChannelStreamOrTrackFormat>>(typedValues)
                .at(descriptor.get())
                .value++};
      }

      // get the next AudioPackFormat value for this TypeDescriptor
      template <>
      AudioPackFormatIdValue next<AudioPackFormat>(TypeDescriptor descriptor) {
        return AudioPackFormatIdValue{
            std::get<TypedIndices<AudioPackFormat>>(typedValues)
                .at(descriptor.get())
                .value++};
      }
    };

    class IdReassigner {
     public:
      IdReassigner(std::shared_ptr<Document> document);

     private:
      void reassignAllIds();
      void reassignAudioPackFormatIds();
      void reassignAudioStreamFormatIds();
      boost::optional<uint32_t> reassignAudioTrackFormatIds(
          const std::shared_ptr<AudioStreamFormat>& audioStreamFormat,
          TypeDescriptor descriptor, boost::optional<uint32_t> idValue);
      void reassignAudioTrackUidIds();
      void reassignAudioBlockFormatIds(
          std::shared_ptr<AudioChannelFormat> audioChannelFormat);

      template <typename T>
      void reassign() {
        undefineIds<T>();
        for (auto const& element : document->template getElements<T>()) {
          assignIdIfNotCommon(element);
        }
      }

      template <typename T>
      void undefineIds() {
        auto elementRange = document->template getElements<T>();
        for (auto const& element : elementRange) {
          using IdT = typename T::id_type;
          auto id = element->template get<IdT>();
          if (!isCommonDefinitionsId(id)) {
            element->set(IdT{});
          }
        }
      }

      template <typename T>
      void assignIdIfNotCommon(std::shared_ptr<T> const& element) {
        auto existingId = element->template get<typename T::id_type>();
        if (!isCommonDefinitionsId(existingId)) {
          element->set(typename T::id_type(values.next<T>()));
        }
      }

      template <typename T>
      boost::optional<uint32_t> assignTypedIdIfNotCommon(
          std::shared_ptr<T> const& element, TypeDescriptor descriptor,
          boost::optional<uint32_t> linkedValue = {}) {
        auto existingId = element->template get<typename T::id_type>();
        if (!isCommonDefinitionsId(existingId)) {
          auto id_value = linkedValue
                              ? typename T::id_type::value_type{*linkedValue}
                              : values.next<T>(descriptor);
          element->set(typename T::id_type(id_value, descriptor));
          linkedValue = id_value.get();
        }
        return linkedValue;
      }

      IdValues values;
      std::shared_ptr<Document> document;
    };
  }  // namespace

  void reassignIds(std::shared_ptr<Document> document) {
    IdReassigner assigner{document};
  }

  IdReassigner::IdReassigner(std::shared_ptr<Document> document)
      : document{document} {
    reassignAllIds();
  }

  void IdReassigner::reassignAllIds() {
    reassign<AudioProgramme>();
    reassign<AudioContent>();
    reassign<AudioObject>();
    reassignAudioPackFormatIds();
    /**
     * Initialize all audioTrackFormatIds and audioChanneldFormatIds to zero.
     * The reason behind this is, that the reassignAudioStreamFormatIds
     * algorithm only gives new IDs to audioTrackFormats and
     * audioChannelFormats which are referenced by an audioStreamFormat. 
     * Additionally, audioStreamFormats are only given a valid ID if they 
     * reference an audioChannelFormat. This should be the right way to do it 
     * for 2076-0/1 structures. Unreferenced elements will get an ID with 
     * value 0 and are thereby marked as elements which should be ignored.
     * For 2076-2 structures, reassignAudioTrackUidIds will apply a unique ID
     * to audioChannelFormats referenced directly from audioTrackUids.
     */
    undefineIds<AudioTrackFormat>();
    undefineIds<AudioChannelFormat>();
    reassignAudioStreamFormatIds();
    reassignAudioTrackUidIds();
  }

  void IdReassigner::reassignAudioPackFormatIds() {
    undefineIds<AudioPackFormat>();
    for (auto audioPackFormat : document->getElements<AudioPackFormat>()) {
      assignTypedIdIfNotCommon(audioPackFormat,
                               audioPackFormat->get<TypeDescriptor>());
    }
  }

  void IdReassigner::reassignAudioStreamFormatIds() {
    undefineIds<AudioStreamFormat>();
    for (auto audioStreamFormat : document->getElements<AudioStreamFormat>()) {
      if (auto audioChannelFormat =
              audioStreamFormat->getReference<AudioChannelFormat>()) {
        auto descriptor = audioChannelFormat->get<TypeDescriptor>();
        auto idValue = assignTypedIdIfNotCommon(audioStreamFormat, descriptor);
        idValue =
            assignTypedIdIfNotCommon(audioChannelFormat, descriptor, idValue);
        if (!isCommonDefinitionsId(
                audioChannelFormat->get<AudioChannelFormatId>())) {
          reassignAudioBlockFormatIds(audioChannelFormat);
        }
        reassignAudioTrackFormatIds(audioStreamFormat, descriptor, idValue);
      }
    }
  }

  boost::optional<uint32_t> IdReassigner::reassignAudioTrackFormatIds(
      std::shared_ptr<AudioStreamFormat> const& audioStreamFormat,
      TypeDescriptor descriptor,
      boost::optional<uint32_t> idValue) {  // AudioTrackFormats
    AudioTrackFormatIdCounter counter{0x01u};
    for (auto& weak : audioStreamFormat->getAudioTrackFormatReferences()) {
      if (auto audioTrackFormat = weak.lock()) {
        auto id = audioTrackFormat->get<AudioTrackFormatId>();
        if (!isCommonDefinitionsId(id)) {
          if (!idValue) {
            idValue = this->values.next<AudioTrackFormat>(descriptor).get();
          }
          id.set(descriptor);
          id.set(AudioTrackFormatIdValue{idValue.get()});
          id.set(counter);
          audioTrackFormat->set(id);
          counter++;
        }
      }
    }
    return idValue;
  }

  namespace {
    std::shared_ptr<AudioChannelFormat> nonCommonChannelFormatReference(
        std::shared_ptr<AudioTrackUid> const& uid) {
      auto acf = uid->getReference<AudioChannelFormat>();
      if (acf && isCommonDefinitionsId(acf->get<AudioChannelFormatId>())) {
        acf = nullptr;
      }
      return acf;
    }
  }  // namespace

  void IdReassigner::reassignAudioTrackUidIds() {
    undefineIds<AudioTrackUid>();
    for (auto uid : document->getElements<AudioTrackUid>()) {
      uid->set(
          AudioTrackUidId(AudioTrackUidIdValue(values.next<AudioTrackUid>())));
      if (auto audioChannelFormat = nonCommonChannelFormatReference(uid)) {
        auto typeDescriptor = audioChannelFormat->get<TypeDescriptor>();
        audioChannelFormat->set(AudioChannelFormatId(
            AudioChannelFormatIdValue{
                values.next<AudioChannelFormat>(typeDescriptor)},
            typeDescriptor));
        reassignAudioBlockFormatIds(audioChannelFormat);
      }
    }
  }

  namespace {
    template <typename T>
    void reassignBlockFormats(std::shared_ptr<AudioChannelFormat> const& acf) {
      auto descriptor = acf->get<TypeDescriptor>();
      auto idValue =
          AudioBlockFormatIdValue(acf->get<AudioChannelFormatId>()
                                      .get<AudioChannelFormatIdValue>()
                                      .get());
      auto idCounter = AudioBlockFormatIdCounter(0x00000001u);
      for (auto& block : acf->getElements<T>()) {
        auto id = block.template get<AudioBlockFormatId>();
        id.set(descriptor);
        id.set(idValue);
        id.set(idCounter);
        block.set(id);
        ++idCounter;
      }
    }
  }  // namespace

  void IdReassigner::reassignAudioBlockFormatIds(
      std::shared_ptr<AudioChannelFormat> audioChannelFormat) {
    auto typeDefinition = audioChannelFormat->get<TypeDescriptor>();
    if (typeDefinition == TypeDefinition::DIRECT_SPEAKERS) {
      reassignBlockFormats<AudioBlockFormatDirectSpeakers>(audioChannelFormat);
    } else if (typeDefinition == TypeDefinition::MATRIX) {
      reassignBlockFormats<AudioBlockFormatMatrix>(audioChannelFormat);
    } else if (typeDefinition == TypeDefinition::OBJECTS) {
      reassignBlockFormats<AudioBlockFormatObjects>(audioChannelFormat);
    } else if (typeDefinition == TypeDefinition::HOA) {
      reassignBlockFormats<AudioBlockFormatHoa>(audioChannelFormat);
    } else if (typeDefinition == TypeDefinition::BINAURAL) {
      reassignBlockFormats<AudioBlockFormatBinaural>(audioChannelFormat);
    }
  }
}  // namespace adm