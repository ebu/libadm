#pragma once
#include "adm/elements.hpp"
#include "adm/serial/frame_header.hpp"
#include "adm/serial/frame_format.hpp"
#include "adm/serial/frame_format_id.hpp"
#include "adm/serial/transport_track_format.hpp"

#include <string>

namespace adm {
  namespace xml {

    class XmlNode;

    void formatAudioProgramme(
        XmlNode &node, const std::shared_ptr<const AudioProgramme> programme);
    void formatLabel(XmlNode &node, const Label &label);
    void formatAudioComplementaryObjectGroupLabel(
        XmlNode &node, const AudioComplementaryObjectGroupLabel &label);
    void formatLoudnessMetadata(XmlNode &node,
                                const LoudnessMetadata loudnessMetadata);
    void formatLoudnessMetadatas(XmlNode &node, const std::string &name,
                                 const LoudnessMetadatas &loudnessMetadatas);
    void formatNonDialogueContentKind(XmlNode &node,
                                      const NonDialogueContentKind contentKind);
    void formatDialogueContentKind(XmlNode &node,
                                   const DialogueContentKind contentKind);
    void formatMixedContentKind(XmlNode &node,
                                const MixedContentKind contentKind);
    void formatAudioContent(XmlNode &node,
                            const std::shared_ptr<const AudioContent> content);
    void formatAudioObject(XmlNode &node,
                           const std::shared_ptr<const AudioObject> object);
    void formatAudioObjectInteraction(
        XmlNode &node, const AudioObjectInteraction &objectInteraction);
    void formatGainInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const GainInteractionRange &gainInteraction);
    void formatPositionInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const PositionInteractionRange &positionInteraction);
    void formatAudioPackFormat(
        XmlNode &node, const std::shared_ptr<const AudioPackFormat> packFormat);
    void formatAudioChannelFormat(
        XmlNode &node,
        const std::shared_ptr<const AudioChannelFormat> channelFormat);
    void formatAudioStreamFormat(
        XmlNode &node,
        const std::shared_ptr<const AudioStreamFormat> streamFormat);
    void formatAudioTrackFormat(
        XmlNode &node,
        const std::shared_ptr<const AudioTrackFormat> trackFormat);
    void formatAudioTrackUid(
        XmlNode &node, const std::shared_ptr<const AudioTrackUid> trackUid);
    void formatProfileList(XmlNode &node, const ProfileList &profileList);
    void formatProfile(XmlNode &node, const Profile &profile);

    void formatBlockFormatDirectSpeakers(
        XmlNode &node, const AudioBlockFormatDirectSpeakers &audioBlock);
    void formatBlockFormatMatrix(XmlNode &node,
                                 const AudioBlockFormatMatrix &audioBlock);
    void formatBlockFormatObjects(XmlNode &node,
                                  const AudioBlockFormatObjects &audioBlock);
    void formatBlockFormatHoa(XmlNode &node,
                              const AudioBlockFormatHoa &audioBlock);
    void formatBlockFormatBinaural(XmlNode &node,
                                   const AudioBlockFormatBinaural &audioBlock);

    void formatSpeakerLabels(XmlNode &parentNode, const std::string &name,
                             const SpeakerLabels &speakerLabels);
    void formatCartesianSpeakerPosition(
        XmlNode &parentNode, const std::string &name,
        const CartesianSpeakerPosition &position);
    void formatSphericalSpeakerPosition(
        XmlNode &parentNode, const std::string &name,
        const SphericalSpeakerPosition &position);
    void formatFrequency(XmlNode &parentNode, const std::string &name,
                         const Frequency &frequency);
    void formatPosition(XmlNode &parentNode, const std::string &name,
                        const Position &position);
    void formatPositionOffset(XmlNode &parentNode, const std::string &name,
                              const PositionOffset &position);
    void formatGain(XmlNode &node, const Gain &gain);
    void formatChannelLock(XmlNode &node, const ChannelLock &channelLock);
    void formatObjectDivergence(XmlNode &node,
                                const ObjectDivergence &objectDivergence);
    void formatJumpPosition(XmlNode &node, const JumpPosition &jumpPosition);
    void formatHeadphoneVirtualise(
        XmlNode &node, const HeadphoneVirtualise &headphoneVirtualise);
    void formatFrameHeader(XmlNode &node, const FrameHeader &header);
    void formatFrameFormat(XmlNode &node, const FrameFormat &format);
    void formatTransportTrackFormat(XmlNode &node,
                                    const TransportTrackFormat &format);
                                    
    namespace detail {
      std::string toString(const std::string &string);
      std::string toString(const Time &time);
      std::string toString(const AudioProgrammeId &id);
      std::string toString(const AudioContentId &id);
      std::string toString(const AudioObjectId &id);
      std::string toString(const AudioPackFormatId &id);
      std::string toString(const AudioChannelFormatId &id);
      std::string toString(const AudioBlockFormatId &id);
      std::string toString(const AudioStreamFormatId &id);
      std::string toString(const AudioTrackFormatId &id);
      std::string toString(const AudioTrackUidId &id);
      std::string toString(const FrameFormatId &id);
      std::string toString(const TransportId &id);
      std::string toString(const FrameType &frameType);

      template <typename T, typename std::enable_if<
                                std::is_integral<T>::value>::type * = nullptr>
      std::string toString(T value) {
        return std::to_string(value);
      }

      template <typename T,
                typename std::enable_if<std::is_floating_point<T>::value>::type
                    * = nullptr>
      std::string toString(T value) {
        return std::to_string(value);
      }

      template <typename T, typename Tag>
      std::string toString(adm::detail::NamedType<T, Tag> value) {
        return toString(value.get());
      }

      template <typename T, typename Tag, typename Validator>
      std::string toString(adm::detail::NamedType<T, Tag, Validator> value) {
        return toString(value.get());
      }
    }  // namespace detail
  }  // namespace xml
}  // namespace adm
