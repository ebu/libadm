#pragma once
#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/errors.hpp"
#include "adm/parse.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "adm/elements/audio_pack_format_hoa.hpp"
#include "adm/detail/id_map.hpp"

namespace adm {
  /**
   * @brief XML parsing implementation
   *
   * This namespace collects parser implementations and helpers for writing
   * parsers.
   *
   * If you're just looking for a way to parse an XML file in client code,
   * refer to `adm::parseXml()`
   */
  namespace xml {
    using NodePtr = rapidxml::xml_node<>*;

    AudioObjectInteraction parseAudioObjectInteraction(NodePtr node);
    GainInteractionRange parseGainInteractionRange(std::vector<NodePtr> nodes);
    PositionInteractionRange parsePositionInteractionRange(
        std::vector<NodePtr> nodes);
    Frequency parseFrequency(std::vector<NodePtr> nodes);
    DialogueId parseDialogueId(NodePtr node);
    ContentKind parseContentKind(NodePtr node);
    Cartesian guessCartesianFlag(NodePtr node, const char* elementName);
    SphericalPosition parseSphericalPosition(std::vector<NodePtr> nodes);
    CartesianPosition parseCartesianPosition(std::vector<NodePtr> nodes);
    SphericalPositionOffset parseSphericalPositionOffset(
        std::vector<NodePtr> nodes);
    CartesianPositionOffset parseCartesianPositionOffset(
        std::vector<NodePtr> nodes);
    LoudnessMetadata parseLoudnessMetadata(NodePtr node);
    LoudnessMetadatas parseLoudnessMetadatas(const std::vector<NodePtr>& nodes);
    AudioProgrammeReferenceScreen parseAudioProgrammeReferenceScreen(
        NodePtr node);
    Label parseLabel(NodePtr node);
    AudioBlockFormatObjects parseAudioBlockFormatObjects(NodePtr node);
    Gain parseGain(NodePtr node);
    ChannelLock parseChannelLock(NodePtr node);
    ObjectDivergence parseObjectDivergence(NodePtr node);
    JumpPosition parseJumpPosition(NodePtr node);
    AudioBlockFormatDirectSpeakers parseAudioBlockFormatDirectSpeakers(
        NodePtr node);
    SphericalSpeakerPosition parseSphericalSpeakerPosition(
        const std::vector<std::pair<NodePtr, SphericalCoordinateValue>>&
            sphericalCoordinates);
    CartesianSpeakerPosition parseCartesianSpeakerPosition(
        const std::vector<
            std::pair<adm::xml::NodePtr, adm::CartesianCoordinateValue>>&
            cartesianCoordinates);
    SpeakerPosition parseSpeakerPosition(std::vector<NodePtr> node);
    SpeakerLabel parseSpeakerLabel(NodePtr node);
    HeadphoneVirtualise parseHeadphoneVirtualise(NodePtr node);
    AudioBlockFormatHoa parseAudioBlockFormatHoa(NodePtr node);
    AudioBlockFormatBinaural parseAudioBlockFormatBinaural(NodePtr node);
    Profile parseProfile(NodePtr node);
    ProfileList parseProfileList(NodePtr node);

    NodePtr findAudioFormatExtendedNodeEbuCore(NodePtr root);
    NodePtr findAudioFormatExtendedNodeFullRecursive(NodePtr root);
    NodePtr findFrameAudioFormatExtended(NodePtr node);

    class DocumentParser {
     public:
      explicit DocumentParser(
          const std::string& filename,
          ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());
      explicit DocumentParser(
          std::istream& stream, ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());

      std::shared_ptr<Document> parse();

      bool hasUnresolvedReferences();

     private:
      explicit DocumentParser(
          rapidxml::file<> file, ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());

      std::shared_ptr<AudioProgramme> parseAudioProgramme(NodePtr node);
      std::shared_ptr<AudioContent> parseAudioContent(NodePtr node);
      std::shared_ptr<AudioObject> parseAudioObject(NodePtr node);
      std::shared_ptr<AudioTrackFormat> parseAudioTrackFormat(NodePtr node);
      std::shared_ptr<AudioStreamFormat> parseAudioStreamFormat(NodePtr node);
      std::shared_ptr<AudioPackFormat> parseAudioPackFormat(NodePtr node);
      std::shared_ptr<AudioTrackUid> parseAudioTrackUid(NodePtr node);
      std::shared_ptr<AudioChannelFormat> parseAudioChannelFormat(NodePtr node);

      rapidxml::file<> xmlFile_;
      ParserOptions options_;
      std::shared_ptr<Document> document_;

      // clang-format off
      std::map<std::shared_ptr<AudioProgramme>, std::vector<AudioContentId>> programmeContentRefs_;
      std::map<std::shared_ptr<AudioContent>, std::vector<AudioObjectId>> contentObjectRefs_;
      std::map<std::shared_ptr<AudioObject>, std::vector<AudioObjectId>> objectObjectRefs_;
      std::map<std::shared_ptr<AudioObject>, std::vector<AudioObjectId>> objectComplementaryObjectRefs_;
      std::map<std::shared_ptr<AudioObject>, std::vector<AudioPackFormatId>> objectPackFormatRefs_;
      std::map<std::shared_ptr<AudioObject>, std::vector<AudioTrackUidId>> objectTrackUidRefs_;
      std::map<std::shared_ptr<AudioTrackUid>, AudioTrackFormatId> trackUidTrackFormatRef_;
      std::map<std::shared_ptr<AudioTrackUid>, AudioChannelFormatId> trackUidChannelFormatRef_;
      std::map<std::shared_ptr<AudioTrackUid>, AudioPackFormatId> trackUidPackFormatRef_;
      std::map<std::shared_ptr<AudioPackFormat>, std::vector<AudioChannelFormatId>> packFormatChannelFormatRefs_;
      std::map<std::shared_ptr<AudioPackFormat>, std::vector<AudioPackFormatId>> packFormatPackFormatRefs_;
      std::map<std::shared_ptr<AudioTrackFormat>, AudioStreamFormatId> trackFormatStreamFormatRef_;
      std::map<std::shared_ptr<AudioStreamFormat>, AudioChannelFormatId> streamFormatChannelFormatRef_;
      std::map<std::shared_ptr<AudioStreamFormat>, AudioPackFormatId> streamFormatPackFormatRef_;
      std::map<std::shared_ptr<AudioStreamFormat>, std::vector<AudioTrackFormatId>> streamFormatTrackFormatRefs_;
      // clang-format on

      /// used to keep track of element IDs ourselves to avoid having it
      /// iterate through the whole document for each element and reference
      ::adm::detail::IDMap idMap_;

      /// add an element to both the document and idMap_
      template <typename Element>
      void add(std::shared_ptr<Element> el);

      template <typename Src, typename TargetId>
      void resolveReferences(const std::map<Src, std::vector<TargetId>>& map) {
        for (const auto& entry : map) {
          for (const auto& id : entry.second) {
            if (auto element = idMap_.lookup(id)) {
              entry.first->addReference(std::move(element));
            } else {
              throw error::XmlParsingUnresolvedReference(formatId(id));
            }
          }
        }
      }

      void resolveTrackUidReferences(
          const std::map<std::shared_ptr<AudioObject>,
                         std::vector<AudioTrackUidId>>& map);

      template <typename Src, typename Target>
      void resolveReference(const std::map<Src, Target>& map) {
        for (const auto& entry : map) {
          const auto& id = entry.second;
          if (auto element = idMap_.lookup(id)) {
            entry.first->setReference(std::move(element));
          } else {
            throw error::XmlParsingUnresolvedReference(formatId(id));
          }
        }
      }
      void setCommonProperties(std::shared_ptr<AudioPackFormat> audioPackFormat,
                               NodePtr node);
    };

  }  // namespace xml
}  // namespace adm