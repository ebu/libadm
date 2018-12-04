/// @file dialogue.hpp
#pragma once

#include <boost/variant.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::DialogueId
  struct DialogueIdTag {};
  /**
   * @brief NamedType for the dialogueId attribute
   *
   * Valid values are in the range [0, 2]
   */
  using DialogueId = detail::NamedType<unsigned int, DialogueIdTag,
                                       detail::RangeValidator<0, 2>>;

  /// DialogueId definitions
  namespace Dialogue {
    const DialogueId NON_DIALOGUE = DialogueId(0);
    /// DialogueId for music
    const DialogueId DIALOGUE = DialogueId(1);
    /// DialogueId for effects
    const DialogueId MIXED = DialogueId(2);
  }  // namespace Dialogue

  /// @brief Tag for NamedType ::NonDialogueContentKind
  struct NonDialogueContentKindTag {};
  /**
   * @brief NamedType for the nonDialogueContentKind type
   *
   * Valid values are in the range [0, 2]
   */
  using NonDialogueContentKind =
      detail::NamedType<unsigned int, NonDialogueContentKindTag,
                        detail::RangeValidator<0, 2>>;

  /// NonDialogueContentKind definitions
  namespace NonDialogueContent {
    const NonDialogueContentKind UNDEFINED = NonDialogueContentKind(0);
    /// NonDialogueContentKind for music
    const NonDialogueContentKind MUSIC = NonDialogueContentKind(1);
    /// NonDialogueContentKind for effects
    const NonDialogueContentKind EFFECT = NonDialogueContentKind(2);
  }  // namespace NonDialogueContent

  /// @brief Tag for NamedType ::DialogueContentKind
  struct DialogueContentKindTag {};
  /**
   * @brief NamedType for the dialogueContentKind type
   *
   * Valid values are in the range [0, 6]
   */
  using DialogueContentKind =
      detail::NamedType<unsigned int, DialogueContentKindTag,
                        detail::RangeValidator<0, 6>>;

  /// DialogueContentKind definitions
  namespace DialogueContent {
    const DialogueContentKind UNDEFINED = DialogueContentKind(0);
    /// DialogueContentKind for (storyline) dialogue
    const DialogueContentKind DIALOGUE = DialogueContentKind(1);
    /// DialogueContentKind for voiceover
    const DialogueContentKind VOICEOVER = DialogueContentKind(2);
    /// DialogueContentKind for spoken subtitle
    const DialogueContentKind SPOKEN_SUBTITLE = DialogueContentKind(3);
    /// DialogueContentKind for audio description/visually impaired
    const DialogueContentKind AUDIO_DESCRIPTION = DialogueContentKind(4);
    /// DialogueContentKind for commentary
    const DialogueContentKind COMMENTARY = DialogueContentKind(5);
    /// DialogueContentKind for emergency
    const DialogueContentKind EMERGENCY = DialogueContentKind(6);
  }  // namespace DialogueContent

  /// @brief Tag for NamedType ::MixedContentKind
  struct MixedContentKindTag {};
  /**
   * @brief NamedType for the mixedContentKind type
   *
   * Valid values are in the range [0, 3]
   */
  using MixedContentKind = detail::NamedType<unsigned int, MixedContentKindTag,
                                             detail::RangeValidator<0, 3>>;

  /// MixedContent definitions
  namespace MixedContent {
    const MixedContentKind UNDEFINED = MixedContentKind(0);
    /// MixedContentKind for complete main
    const MixedContentKind COMPLETE_MAIN = MixedContentKind(1);
    /// MixedContentKind for mixed
    const MixedContentKind MIXED = MixedContentKind(2);
    /// MixedContentKind for hearing impaired
    const MixedContentKind HEARING_IMPAIRED = MixedContentKind(3);
  }  // namespace MixedContent

  ///@brief Type to hold a NonDialogueContentKind, DialogueContentKind or
  /// MixedContentKind
  typedef boost::variant<NonDialogueContentKind, DialogueContentKind,
                         MixedContentKind>
      ContentKind;

  ADD_TRAIT(ContentKind, ContentKindTag);

}  // namespace adm
