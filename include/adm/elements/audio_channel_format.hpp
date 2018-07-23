/// @file audio_channel_format.hpp
#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <memory>
#include <vector>
#include "adm/elements/audio_block_format_binaural.hpp"
#include "adm/elements/audio_block_format_direct_speakers.hpp"
#include "adm/elements/audio_block_format_hoa.hpp"
#include "adm/elements/audio_block_format_matrix.hpp"
#include "adm/elements/audio_block_format_objects.hpp"
#include "adm/elements/audio_channel_format_id.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/libadm_export.h"
#include <type_traits>

namespace adm {

  class Document;

  /**
   * Helper to deduce the correct `IteratorRange` type for
   * `const_iterators` to the requested `AudioBlockFormat` type
   * @headerfile audio_channel_format.hpp <adm/elements/channel_format.hpp>
   */
  template <typename AudioBlockFormat>
  using BlockFormatsConstRange = boost::iterator_range<
      typename std::vector<AudioBlockFormat>::const_iterator>;

  /**
   * Helper  to deduce the correct `IteratorRange` type for
   * `iterators` to the requested `AudioBlockFormat` type
   * @headerfile audio_channel_format.hpp <adm/elements/channel_format.hpp>
   */
  template <typename AudioBlockFormat>
  using BlockFormatsRange =
      boost::iterator_range<typename std::vector<AudioBlockFormat>::iterator>;

  /// @brief Tag for NamedType ::AudioChannelFormatName
  struct AudioChannelFormatNameTag {};
  /// @brief NamedType for the audioChannelFormatName attribute
  using AudioChannelFormatName =
      detail::NamedType<std::string, AudioChannelFormatNameTag>;

  /// @brief Tag for AudioChannelFormat
  struct AudioChannelFormatTag {};
  /**
   * @brief Class representation of the audioChannelFormat ADM element
   *
   * @headerfile audio_channel_format.hpp \
   * <adm/elements/audio_channel_format.hpp>
   */
  class AudioChannelFormat
      : public std::enable_shared_from_this<AudioChannelFormat> {
   public:
    typedef AudioChannelFormatTag tag;
    /// Type that holds the id for this element;
    typedef AudioChannelFormatId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioChannelFormat object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioChannelFormat> create(
        AudioChannelFormatName name, TypeDescriptor channelType,
        Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioChannelFormat
     *
     * The actual copy constructor is private to ensure that an
     * AudioChannelFormat can only be created as a `std::shared_ptr`. Added
     * AudioBlockFormats will be copied too.
     */
    LIBADM_EXPORT std::shared_ptr<AudioChannelFormat> copy() const;

    /**
     * @brief ADM parameter getter template
     *
     * Templated getter with the wanted ADM parameter type as template
     * argument. If currently no value is available trying to get the adm
     * parameter will result in an exception. Check with the has method before
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as template argument.
     * Returns true if the ADM parameter is set or has a default value.
     */
    template <typename Parameter>
    bool has() const;

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as template
     * argument. Returns true if the ADM parameter is the default value.
     */
    template <typename Parameter>
    bool isDefault() const;

    /// @brief AudioChannelFormatId setter
    LIBADM_EXPORT void set(AudioChannelFormatId id);
    /// @brief AudioChannelFormatName setter
    LIBADM_EXPORT void set(AudioChannelFormatName name);
    /// @brief Frequency setter
    LIBADM_EXPORT void set(Frequency frequency);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    /**
     * @brief Add AudioBlockFormats
     *
     * The AudioBlockFormat has to be of the correct type. Otherwise an
     * exception is thrown.
     */
    LIBADM_EXPORT void add(AudioBlockFormatDirectSpeakers blockFormat);
    LIBADM_EXPORT void add(AudioBlockFormatMatrix blockFormat);
    LIBADM_EXPORT void add(AudioBlockFormatObjects blockFormat);
    LIBADM_EXPORT void add(AudioBlockFormatHoa blockFormat);
    LIBADM_EXPORT void add(AudioBlockFormatBinaural blockFormat);

    /**
     * @brief AudioBlockFormat elements getter template
     *
     * Templated getter with the wanted audioBlockFormat type as template
     * argument.
     *
     * @returns ContainerProxy containing all audioBlockFormats of the given
     * type.
     */
    template <typename AudioBlockFormat>
    BlockFormatsConstRange<AudioBlockFormat> getElements() const;

    /**
     * @brief AudioBlockFormat elements getter template
     *
     * Templated getter with the wanted audioBlockFormat type as template
     * argument.
     *
     * @returns ContainerProxy containing all audioBlockFormats of the given
     * type.
     */
    template <typename AudioBlockFormat>
    BlockFormatsRange<AudioBlockFormat> getElements();

    /**
     * @brief Clear AudioBlockFormats
     *
     * Removes all audioBlockFormats from the AudioChannelFormat
     */
    LIBADM_EXPORT void clearAudioBlockFormats();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    LIBADM_EXPORT std::weak_ptr<Document> getParent() const;

   private:
    friend class AudioChannelFormatAttorney;

    LIBADM_EXPORT AudioChannelFormat(AudioChannelFormatName name,
                                     TypeDescriptor channelType);
    LIBADM_EXPORT AudioChannelFormat(const AudioChannelFormat &) = default;
    LIBADM_EXPORT AudioChannelFormat(AudioChannelFormat &&) = default;

    LIBADM_EXPORT AudioChannelFormatId
        get(detail::ParameterTraits<AudioChannelFormatId>::tag) const;
    LIBADM_EXPORT AudioChannelFormatName
        get(detail::ParameterTraits<AudioChannelFormatName>::tag) const;
    LIBADM_EXPORT TypeDescriptor
        get(detail::ParameterTraits<TypeDescriptor>::tag) const;
    LIBADM_EXPORT Frequency get(detail::ParameterTraits<Frequency>::tag) const;

    LIBADM_EXPORT bool has(
        detail::ParameterTraits<AudioChannelFormatId>::tag) const;
    LIBADM_EXPORT bool has(
        detail::ParameterTraits<AudioChannelFormatName>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<TypeDescriptor>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Frequency>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    LIBADM_EXPORT void unset(detail::ParameterTraits<Frequency>::tag);

    // ----- AudioBlockFormats ----- //
    LIBADM_EXPORT bool add(
        detail::ParameterTraits<AudioBlockFormatDirectSpeakers>::tag,
        const AudioBlockFormatDirectSpeakers &blockFormat);
    LIBADM_EXPORT bool add(detail::ParameterTraits<AudioBlockFormatMatrix>::tag,
                           const AudioBlockFormatMatrix &blockFormat);
    LIBADM_EXPORT bool add(
        detail::ParameterTraits<AudioBlockFormatObjects>::tag,
        const AudioBlockFormatObjects &blockFormat);
    LIBADM_EXPORT bool add(detail::ParameterTraits<AudioBlockFormatHoa>::tag,
                           const AudioBlockFormatHoa &blockFormat);
    LIBADM_EXPORT bool add(
        detail::ParameterTraits<AudioBlockFormatBinaural>::tag,
        const AudioBlockFormatBinaural &blockFormat);

    template <typename BlockFormat>
    void assignId(BlockFormat &blockFormat);

    template <typename BlockFormat>
    bool idUsed(const AudioBlockFormatId &id);

    template <typename BlockFormatProxy>
    void assignNewIdValue();

    LIBADM_EXPORT
    BlockFormatsConstRange<AudioBlockFormatDirectSpeakers> get(
        detail::ParameterTraits<AudioBlockFormatDirectSpeakers>::tag) const;
    LIBADM_EXPORT
    BlockFormatsConstRange<AudioBlockFormatMatrix> get(
        detail::ParameterTraits<AudioBlockFormatMatrix>::tag) const;
    LIBADM_EXPORT
    BlockFormatsConstRange<AudioBlockFormatObjects> get(
        detail::ParameterTraits<AudioBlockFormatObjects>::tag) const;
    LIBADM_EXPORT
    BlockFormatsConstRange<AudioBlockFormatHoa> get(
        detail::ParameterTraits<AudioBlockFormatHoa>::tag) const;
    LIBADM_EXPORT
    BlockFormatsConstRange<AudioBlockFormatBinaural> get(
        detail::ParameterTraits<AudioBlockFormatBinaural>::tag) const;

    LIBADM_EXPORT
    BlockFormatsRange<AudioBlockFormatDirectSpeakers> get(
        detail::ParameterTraits<AudioBlockFormatDirectSpeakers>::tag);
    LIBADM_EXPORT
    BlockFormatsRange<AudioBlockFormatMatrix> get(
        detail::ParameterTraits<AudioBlockFormatMatrix>::tag);
    LIBADM_EXPORT
    BlockFormatsRange<AudioBlockFormatObjects> get(
        detail::ParameterTraits<AudioBlockFormatObjects>::tag);
    LIBADM_EXPORT
    BlockFormatsRange<AudioBlockFormatHoa> get(
        detail::ParameterTraits<AudioBlockFormatHoa>::tag);
    LIBADM_EXPORT
    BlockFormatsRange<AudioBlockFormatBinaural> get(
        detail::ParameterTraits<AudioBlockFormatBinaural>::tag);

    // ----- Common ----- //
    LIBADM_EXPORT void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    AudioChannelFormatName name_;
    TypeDescriptor typeDescriptor_;
    AudioChannelFormatId id_;
    boost::optional<Frequency> frequency_;

    std::vector<AudioBlockFormatDirectSpeakers>
        audioBlockFormatsDirectSpeakers_;
    std::vector<AudioBlockFormatMatrix> audioBlockFormatsMatrix_;
    std::vector<AudioBlockFormatObjects> audioBlockFormatsObjects_;
    std::vector<AudioBlockFormatHoa> audioBlockFormatsHoa_;
    std::vector<AudioBlockFormatBinaural> audioBlockFormatsBinaural_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioChannelFormat> AudioChannelFormat::create(
      AudioChannelFormatName name, TypeDescriptor channelType,
      Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioChannelFormat> channel(
        new AudioChannelFormat(name, channelType));
    detail::setNamedOptionHelper(
        channel, std::forward<Parameters>(optionalNamedArgs)...);
    return channel;
  }

  template <typename Parameter>
  Parameter AudioChannelFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioChannelFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioChannelFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioChannelFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename AudioBlockFormat>
  BlockFormatsConstRange<AudioBlockFormat> AudioChannelFormat::getElements()
      const {
    typedef typename detail::ParameterTraits<AudioBlockFormat>::tag Tag;
    return get(Tag());
  }

  template <typename AudioBlockFormat>
  BlockFormatsRange<AudioBlockFormat> AudioChannelFormat::getElements() {
    typedef typename detail::ParameterTraits<AudioBlockFormat>::tag Tag;
    return get(Tag());
  }

  template <typename BlockFormatProxy>
  void AudioChannelFormat::assignNewIdValue() {
    for (auto &blockFormat : getElements<BlockFormatProxy>()) {
      auto blockFormatId = blockFormat.template get<AudioBlockFormatId>();
      auto channelFormatIdValue = get<AudioChannelFormatId>()
                                      .template get<AudioChannelFormatIdValue>()
                                      .get();
      blockFormatId.set(AudioBlockFormatIdValue(channelFormatIdValue));
      blockFormat.set(blockFormatId);
    }
  }

  template <typename BlockFormat>
  bool AudioChannelFormat::idUsed(const AudioBlockFormatId &id) {
    auto blockFormats = getElements<BlockFormat>();
    auto it = std::find_if(
        blockFormats.begin(), blockFormats.end(),
        [&id](const BlockFormat &blockFormat) {
          return blockFormat.template get<AudioBlockFormatId>() == id;
        });
    if (it != blockFormats.end()) {
      return true;
    } else {
      return false;
    }
  }

  template <typename BlockFormat>
  void AudioChannelFormat::assignId(BlockFormat &blockFormat) {
    auto id = blockFormat.template get<AudioBlockFormatId>();

    TypeDescriptor typeDescriptor;
    AudioBlockFormatIdValue value;
    AudioBlockFormatIdCounter counter;

    if (isUndefined(id)) {
      typeDescriptor = get<TypeDescriptor>();
      value = AudioBlockFormatIdValue(
          get<AudioChannelFormatId>().get<AudioChannelFormatIdValue>().get());
      counter = AudioBlockFormatIdCounter(1u);
    } else {
      typeDescriptor = id.template get<TypeDescriptor>();
      value = id.template get<AudioBlockFormatIdValue>();
      counter = id.template get<AudioBlockFormatIdCounter>();
    }

    while (idUsed<BlockFormat>(
        AudioBlockFormatId(typeDescriptor, value, counter))) {
      ++counter;
    }
    blockFormat.set(AudioBlockFormatId(typeDescriptor, value, counter));
  }

}  // namespace adm
