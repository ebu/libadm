/// @file audio_block_format_objects.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/channel_lock.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/jump_position.hpp"
#include "adm/elements/object_divergence.hpp"
#include "adm/elements/position.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/elements/screen_ref.hpp"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::Cartesian
  struct CartesianTag {};
  /// @brief NamedType for cartesian parameter
  using Cartesian = detail::NamedType<bool, CartesianTag>;
  /// @brief Tag for NamedType ::Width
  struct WidthTag {};
  /// @brief NamedType for width parameter
  using Width = detail::NamedType<float, WidthTag>;
  /// @brief Tag for NamedType ::Height
  struct HeightTag {};
  /// @brief NamedType for height parameter
  using Height = detail::NamedType<float, HeightTag>;
  /// @brief Tag for NamedType ::Depth
  struct DepthTag {};
  /// @brief NamedType for depth parameter
  using Depth = detail::NamedType<float, DepthTag>;
  /// @brief Tag for NamedType ::Gain
  struct GainTag {};
  /// @brief NamedType for gain parameter
  using Gain = detail::NamedType<float, GainTag>;
  /// @brief Tag for NamedType ::Diffuse
  struct DiffuseTag {};
  /// @brief NamedType for diffuse parameter
  using Diffuse =
      detail::NamedType<float, DiffuseTag, detail::RangeValidator<0, 1>>;

  /// @brief Tag for AudioBlockFormatObjects
  struct AudioBlockFormatObjectsTag {};
  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "Objects"
   *
   * @warning This class has unsupported parameters
   *   - ZoneExclusion
   */
  class AudioBlockFormatObjects {
   public:
    typedef AudioBlockFormatObjectsTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    AudioBlockFormatObjects(CartesianPosition position,
                            Parameters... optionalNamedArgs);
    template <typename... Parameters>
    AudioBlockFormatObjects(SphericalPosition position,
                            Parameters... optionalNamedArgs);
    ADM_EXPORT AudioBlockFormatObjects(const AudioBlockFormatObjects&) =
        default;
    ADM_EXPORT AudioBlockFormatObjects(AudioBlockFormatObjects&&) = default;
    ADM_EXPORT AudioBlockFormatObjects& operator=(
        const AudioBlockFormatObjects&) = default;
    ADM_EXPORT AudioBlockFormatObjects& operator=(AudioBlockFormatObjects&&) =
        default;

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

    /// @brief AudioBlockFormatId setter
    ADM_EXPORT void set(AudioBlockFormatId id);
    /// @brief Rtime setter
    ADM_EXPORT void set(Rtime rtime);
    /// @brief Duration setter
    ADM_EXPORT void set(Duration duration);

    /**
     * @brief Cartesian setter
     *
     * \note If setting the ADM parameter cartesian will change the coordinate
     * system the corresponding position will be set to a default value.
     */
    ADM_EXPORT void set(Cartesian cartesian);
    /**
     * @brief Position setter
     *
     * \note Setting a SphericalPosition will automatically unset the
     * CartesianPosition and the other way around. If a CartesianPosition is
     * set the Cartesian flag will be set too. If a SphericalPosition is set
     * the Cartesian flag will only be set if has already been set before
     * (either directly or automatically).
     */
    ADM_EXPORT void set(Position position);
    /**
     * @brief SphericalPosition setter
     *
     * \note Setting a SphericalPosition will automatically unset the
     * CartesianPosition. The Cartesian flag will only be set if it has
     * already been set before (either directly or automatically).
     */
    ADM_EXPORT void set(SphericalPosition position);
    /**
     * @brief CartesianPosition setter
     *
     * \note Setting a CartesianPosition will automatically unset the
     * SphericalPosition. Also the Cartesian flag will be set automatically.
     */
    ADM_EXPORT void set(CartesianPosition position);
    /// @brief Width setter
    ADM_EXPORT void set(Width width);
    /// @brief Height setter
    ADM_EXPORT void set(Height height);
    /// @brief Depth setter
    ADM_EXPORT void set(Depth depth);
    /// @brief ScreenEdgeLock setter
    ADM_EXPORT void set(ScreenEdgeLock screenEdgeLock);
    /// @brief Gain setter
    ADM_EXPORT void set(Gain gain);
    /// @brief Diffuse setter
    ADM_EXPORT void set(Diffuse diffuse);
    /// @brief ChannelLock setter
    ADM_EXPORT void set(ChannelLock channelLock);
    /// @brief ObjectDivergence setter
    ADM_EXPORT void set(ObjectDivergence objectDivergence);
    /// @brief JumpPosition setter
    ADM_EXPORT void set(JumpPosition jumpPosition);
    /// @brief ScreenRef setter
    ADM_EXPORT void set(ScreenRef screenRef);
    /// @brief Importance setter
    ADM_EXPORT void set(Importance importance);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

   private:
    ADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT Cartesian get(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT Position get(detail::ParameterTraits<Position>::tag) const;
    ADM_EXPORT SphericalPosition
        get(detail::ParameterTraits<SphericalPosition>::tag) const;
    ADM_EXPORT CartesianPosition
        get(detail::ParameterTraits<CartesianPosition>::tag) const;
    ADM_EXPORT Width get(detail::ParameterTraits<Width>::tag) const;
    ADM_EXPORT Height get(detail::ParameterTraits<Height>::tag) const;
    ADM_EXPORT Depth get(detail::ParameterTraits<Depth>::tag) const;
    ADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;
    ADM_EXPORT Gain get(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT Diffuse get(detail::ParameterTraits<Diffuse>::tag) const;
    ADM_EXPORT ChannelLock get(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT ObjectDivergence
        get(detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT JumpPosition
        get(detail::ParameterTraits<JumpPosition>::tag) const;
    ADM_EXPORT ScreenRef get(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT Importance get(detail::ParameterTraits<Importance>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Position>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<SphericalPosition>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<CartesianPosition>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Width>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Height>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Depth>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Diffuse>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<JumpPosition>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Importance>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Width>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Height>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Depth>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Diffuse>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<JumpPosition>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Importance>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Cartesian>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Position>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<SphericalPosition>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<CartesianPosition>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Width>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Height>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Depth>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Gain>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Diffuse>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ChannelLock>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ObjectDivergence>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<JumpPosition>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenRef>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Importance>::tag);

    AudioBlockFormatId id_;
    boost::optional<Rtime> rtime_;
    boost::optional<Duration> duration_;
    boost::optional<Cartesian> cartesian_;
    boost::optional<SphericalPosition> sphericalPosition_;
    boost::optional<CartesianPosition> cartesianPosition_;
    boost::optional<Height> height_;
    boost::optional<Width> width_;
    boost::optional<Depth> depth_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;
    boost::optional<Gain> gain_;
    boost::optional<Diffuse> diffuse_;
    boost::optional<ChannelLock> channelLock_;
    boost::optional<ObjectDivergence> objectDivergence_;
    boost::optional<JumpPosition> jumpPosition_;
    boost::optional<ScreenRef> screenRef_;
    boost::optional<Importance> importance_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatObjects::AudioBlockFormatObjects(
      CartesianPosition position, Parameters... optionalNamedArgs) {
    set(position);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename... Parameters>
  AudioBlockFormatObjects::AudioBlockFormatObjects(
      SphericalPosition position, Parameters... optionalNamedArgs) {
    set(position);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter AudioBlockFormatObjects::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatObjects::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatObjects::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioBlockFormatObjects::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
