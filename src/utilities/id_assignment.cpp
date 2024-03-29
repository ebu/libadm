#include "adm/utilities/id_assignment.hpp"

namespace adm {
  /**
   * @brief Reassigns ID's to all elements within a Document.
   *
   * Works by clearing existing ID's and then reassigning unique
   * ID's to each element. For AudioTrackFormat, AudioStreamFormat
   * and AudioChannelFormat, new ID's are only assigned if they
   * form a functional part of the ADM though releationships to
   * other elements. Those that don't do not get assigned new ID's
   * after being cleared and are therefore marked as elements to
   * be ignored.
   * 
   * @note This class differs from IdAssigner in that it is more
   * efficient for this purpose. IdAssigner uses lookups to find
   * an available ID, which has exponential complexity. 
   * IdReassigner uses the IdIssuer class to track ID's through
   * simple incrementation and so has linear complexity.
   * 
   * @param document Document that will have element ID's 
   * reassigned.
   */
  class IdReassigner {
   public:
    IdReassigner(std::shared_ptr<Document> document);
    void reassignAllIds();

   private:
    void reassignAudioProgrammeIds();
    void reassignAudioContentIds();
    void reassignAudioObjectIds();
    void reassignAudioPackFormatIds();
    void reassignAudioStreamFormatIds();
    void reassignAudioTrackUidIds();
    void reassignAudioBlockFormatIds(
        std::shared_ptr<AudioChannelFormat> audioChannelFormat);

    /**
     * @brief Class responsible for issuing new ID's for elements.
     *
     * Starts at initial ID values and counts up as ID's are 
     * handed out. Ensures ID's are sequential and unique.
     * A single ID value can also be issued for a related group
     * of AudioTrackFormat, AudioStreamFormat and 
     * AudioChannelFormat elements to make the ADM XML easier to
     * follow.
     */
    class IdIssuer {
     public:
      IdIssuer();
      AudioProgrammeId issueAudioProgrammeId();
      AudioContentId issueAudioContentId();
      AudioObjectId issueAudioObjectId();
      AudioTrackUidId issueAudioTrackUidId();
      AudioPackFormatId issueAudioPackFormatId(TypeDescriptor typeDescriptor);
      AudioChannelFormatId issueAudioChannelFormatId(
          TypeDescriptor typeDescriptor);
      uint16_t issueAudioChannelStreamTrackFormatIdValue(
          TypeDescriptor typeDescriptor);

     private:
      uint32_t nextAudioProgrammeIdValue{0x1001u};
      uint32_t nextAudioContentIdValue{0x1001u};
      uint32_t nextAudioObjectIdValue{0x1001u};
      uint64_t nextAudioTrackUidIdValue{0x00000001u};
      std::map<TypeDescriptor, uint32_t> nextAudioPackFormatIdValue;
      std::map<TypeDescriptor, uint32_t>
          nextAudioChannelStreamTrackFormatIdValue;

    } idIssuer;

    std::shared_ptr<Document> document;
  };

  template <typename It>
  void undefineIds(It begin, It end) {
    for (auto it = begin; it != end; ++it) {
      auto element = *it;
      using ElementId = typename decltype(element)::element_type::id_type;
      auto id = element->template get<ElementId>();
      if (!isCommonDefinitionsId(id)) {
        element->set(ElementId());
      }
    }
  }

  void reassignIds(std::shared_ptr<Document> document) {
    IdReassigner idReassigner(document);
    idReassigner.reassignAllIds();
  }

  IdReassigner::IdReassigner(std::shared_ptr<Document> document)
      : document{document} {}

  void IdReassigner::reassignAllIds() {
    reassignAudioProgrammeIds();
    reassignAudioContentIds();
    reassignAudioObjectIds();
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
    undefineIds(document->getElements<AudioTrackFormat>().begin(),
                document->getElements<AudioTrackFormat>().end());
    undefineIds(document->getElements<AudioChannelFormat>().begin(),
                document->getElements<AudioChannelFormat>().end());
    reassignAudioStreamFormatIds();
    reassignAudioTrackUidIds();
  }

  void IdReassigner::reassignAudioProgrammeIds() {
    auto audioProgrammes = document->getElements<AudioProgramme>();
    undefineIds(audioProgrammes.begin(), audioProgrammes.end());
    for (auto audioProgramme : audioProgrammes) {
      auto audioProgrammeId = audioProgramme->get<AudioProgrammeId>();
      if (!isCommonDefinitionsId(audioProgrammeId)) {
        audioProgramme->set(idIssuer.issueAudioProgrammeId());
      }
    }
  }

  void IdReassigner::reassignAudioContentIds() {
    auto audioContents = document->getElements<AudioContent>();
    undefineIds(audioContents.begin(), audioContents.end());
    for (auto audioContent : audioContents) {
      auto audioContentId = audioContent->get<AudioContentId>();
      if (!isCommonDefinitionsId(audioContentId)) {
        audioContent->set(idIssuer.issueAudioContentId());
      }
    }
  }

  void IdReassigner::reassignAudioObjectIds() {
    auto audioObjects = document->getElements<AudioObject>();
    undefineIds(audioObjects.begin(), audioObjects.end());
    for (auto audioObject : audioObjects) {
      auto audioObjectId = audioObject->get<AudioObjectId>();
      if (!isCommonDefinitionsId(audioObjectId)) {
        audioObject->set(idIssuer.issueAudioObjectId());
      }
    }
  }

  void IdReassigner::reassignAudioPackFormatIds() {
    auto audioPackFormats = document->getElements<AudioPackFormat>();
    undefineIds(audioPackFormats.begin(), audioPackFormats.end());
    for (auto audioPackFormat : audioPackFormats) {
      auto typeDescriptor = audioPackFormat->get<TypeDescriptor>();
      auto audioPackFormatId = audioPackFormat->get<AudioPackFormatId>();
      if (!isCommonDefinitionsId(audioPackFormatId)) {
        audioPackFormat->set(idIssuer.issueAudioPackFormatId(typeDescriptor));
      }
    }
  }

  void IdReassigner::reassignAudioStreamFormatIds() {
    auto audioStreamFormats = document->getElements<AudioStreamFormat>();
    undefineIds(audioStreamFormats.begin(), audioStreamFormats.end());
    for (auto audioStreamFormat : audioStreamFormats) {
      auto audioChannelFormat =
          audioStreamFormat->getReference<AudioChannelFormat>();
      if (!audioChannelFormat) {
        // If no ACF, don't process this tree - it's redundant
        continue;
      }
      auto typeDescriptor = audioChannelFormat->get<TypeDescriptor>();
      uint16_t idValue =
          0;  // don't issue unless needed - 0 (invalid) denotes unset

      // AudioStreamFormat
      auto audioStreamFormatId = audioStreamFormat->get<AudioStreamFormatId>();
      if (!isCommonDefinitionsId(audioStreamFormatId)) {
        if (idValue == 0)
          idValue = idIssuer.issueAudioChannelStreamTrackFormatIdValue(
              typeDescriptor);
        audioStreamFormatId.set(typeDescriptor);
        audioStreamFormatId.set(AudioStreamFormatIdValue{idValue});
        audioStreamFormat->set(audioStreamFormatId);
      }

      // AudioChannelFormat
      auto audioChannelFormatId =
          audioChannelFormat->get<AudioChannelFormatId>();
      if (!isCommonDefinitionsId(audioChannelFormatId)) {
        if (idValue == 0)
          idValue = idIssuer.issueAudioChannelStreamTrackFormatIdValue(
              typeDescriptor);
        audioChannelFormatId.set(typeDescriptor);
        audioChannelFormatId.set(AudioChannelFormatIdValue{idValue});
        audioChannelFormat->set(audioChannelFormatId);
        reassignAudioBlockFormatIds(audioChannelFormat);
      }

      // AudioTrackFormats
      AudioTrackFormatIdCounter audioTrackFormatIdCounter{0x01u};
      for (auto& weakAudioTrackFormat :
           audioStreamFormat->getAudioTrackFormatReferences()) {
        auto audioTrackFormat = weakAudioTrackFormat.lock();
        if (!audioTrackFormat) {
          continue;
        }
        auto audioTrackFormatId = audioTrackFormat->get<AudioTrackFormatId>();
        if (!isCommonDefinitionsId(audioTrackFormatId)) {
          if (idValue == 0)
            idValue = idIssuer.issueAudioChannelStreamTrackFormatIdValue(
                typeDescriptor);
          if (audioTrackFormatIdCounter > 0xFFu)
            throw std::runtime_error("No AudioTrackFormatIdCounter available");
          audioTrackFormatId.set(typeDescriptor);
          audioTrackFormatId.set(AudioTrackFormatIdValue{idValue});
          audioTrackFormatId.set(audioTrackFormatIdCounter);
          audioTrackFormat->set(audioTrackFormatId);
          audioTrackFormatIdCounter++;
        }
      }
    }
  }

  void IdReassigner::reassignAudioTrackUidIds() {
    auto audioTrackUids = document->getElements<AudioTrackUid>();
    undefineIds(audioTrackUids.begin(), audioTrackUids.end());
    for (auto audioTrackUid : audioTrackUids) {
      audioTrackUid->set(idIssuer.issueAudioTrackUidId());
      auto audioChannelFormat =
          audioTrackUid->getReference<adm::AudioChannelFormat>();
      if (audioChannelFormat) {
        auto audioChannelFormatId =
            audioChannelFormat->get<AudioChannelFormatId>();
        if (!isCommonDefinitionsId(audioChannelFormatId)) {
          auto typeDescriptor = audioChannelFormat->get<TypeDescriptor>();
          audioChannelFormat->set(
              idIssuer.issueAudioChannelFormatId(typeDescriptor));
          reassignAudioBlockFormatIds(audioChannelFormat);
        }
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

  IdReassigner::IdIssuer::IdIssuer() {}
  AudioProgrammeId IdReassigner::IdIssuer::issueAudioProgrammeId() {
    if (nextAudioProgrammeIdValue > 0xFFFFu)
      throw std::runtime_error("No AudioProgrammeId available");
    AudioProgrammeId id;
    id.set(AudioProgrammeIdValue(nextAudioProgrammeIdValue++));
    return id;
  }

  AudioContentId IdReassigner::IdIssuer::issueAudioContentId() {
    if (nextAudioContentIdValue > 0xFFFFu)
      throw std::runtime_error("No AudioContentId available");
    AudioContentId id;
    id.set(AudioContentIdValue(nextAudioContentIdValue++));
    return id;
  }

  AudioObjectId IdReassigner::IdIssuer::issueAudioObjectId() {
    if (nextAudioObjectIdValue > 0xFFFFu)
      throw std::runtime_error("No AudioObjectId available");
    AudioObjectId id;
    id.set(AudioObjectIdValue(nextAudioObjectIdValue++));
    return id;
  }

  AudioTrackUidId IdReassigner::IdIssuer::issueAudioTrackUidId() {
    if (nextAudioTrackUidIdValue > 0xFFFFFFFFu)
      throw std::runtime_error("No AudioTrackUidId available");
    AudioTrackUidId id;
    id.set(AudioTrackUidIdValue(
        static_cast<uint32_t>(nextAudioTrackUidIdValue++)));
    return id;
  }

  AudioPackFormatId IdReassigner::IdIssuer::issueAudioPackFormatId(
      TypeDescriptor typeDescriptor) {
    auto it = nextAudioPackFormatIdValue.emplace(typeDescriptor, 0x1001).first;
    if (it->second > 0xFFFFu)
      throw std::runtime_error("No AudioPackFormatId available");
    AudioPackFormatId id;
    id.set(typeDescriptor);
    id.set(AudioPackFormatIdValue(it->second++));
    return id;
  }

  AudioChannelFormatId IdReassigner::IdIssuer::issueAudioChannelFormatId(
      TypeDescriptor typeDescriptor) {
    auto it =
        nextAudioChannelStreamTrackFormatIdValue.emplace(typeDescriptor, 0x1001)
            .first;
    if (it->second > 0xFFFFu)
      throw std::runtime_error("No AudioChannelFormatId available");
    AudioChannelFormatId id;
    id.set(typeDescriptor);
    id.set(AudioChannelFormatIdValue(it->second++));
    return id;
  }
  uint16_t IdReassigner::IdIssuer::issueAudioChannelStreamTrackFormatIdValue(
      TypeDescriptor typeDescriptor) {
    auto it =
        nextAudioChannelStreamTrackFormatIdValue.emplace(typeDescriptor, 0x1001)
            .first;
    if (it->second > 0xFFFFu)
      throw std::runtime_error(
          "No common AudioChannelFormat, AudioStreamFormat, AudioTrackFormat "
          "ID value available");
    return static_cast<uint16_t>(it->second++);
  }

}  // namespace adm
