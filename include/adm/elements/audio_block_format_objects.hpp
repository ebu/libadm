/// @file audio_block_format_objects.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/channel_lock.hpp"
#include "adm/elements/jump_position.hpp"
#include "adm/elements/object_divergence.hpp"
#include "adm/elements/position.hpp"
#include "adm/elements/position_offset.hpp"
#include "adm/elements/common_parameters.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/auto_base.hpp"
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
  /// @brief Tag for NamedType ::Diffuse
  struct DiffuseTag {};
  /// @brief NamedType for diffuse parameter
  using Diffuse =
      detail::NamedType<float, DiffuseTag, detail::RangeValidator<0, 1>>;

  /// @brief Tag for AudioBlockFormatObjects
  struct AudioBlockFormatObjectsTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Width>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Height>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Depth>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Diffuse>;

    using AudioBlockFormatObjectsBase = HasParameters<
        RequiredParameter<AudioBlockFormatId>, DefaultParameter<Rtime>,
        OptionalParameter<Duration>, OptionalParameter<Lstart>,
        OptionalParameter<Lduration>, 
        OptionalParameter<InitializeBlock>, DefaultParameter<Width>,
        DefaultParameter<Height>, DefaultParameter<Depth>,
        DefaultParameter<Diffuse>, DefaultParameter<Gain>,
        DefaultParameter<Importance>, DefaultParameter<HeadphoneVirtualise>,
        DefaultParameter<HeadLocked>, DefaultParameter<ScreenRef>>;
  }  // namespace detail

  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "Objects"
   *
   * Supported parameters are as follows:
   *
   * \rst
   * +---------------------+------------------------------+----------------------------+
   * | ADM Parameter       | Parameter Type               | Pattern Type               |
   * +=====================+==============================+============================+
   * | audioBlockFormatId  | :class:`AudioBlockFormatId`  | :class:`RequiredParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | rtime               | :type:`Rtime`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | duration            | :type:`Duration`             | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lstart              | :type:`LStart`               | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lduration           | :type:`Lduration`            | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | initializeBlock     | :type:`InitializeBlock`      | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | cartesian           | :type:`Cartesian`            | custom, see below          |
   * +---------------------+------------------------------+                            |
   * | position            | - :type:`Position`           |                            |
   * |                     | - :type:`SphericalPosition`  |                            |
   * |                     | - :type:`CartesianPosition`  |                            |
   * +---------------------+------------------------------+----------------------------+
   * | width               | :type:`Width`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | height              | :type:`Height`               | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | depth               | :type:`Depth`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | diffuse             | :type:`Diffuse`              | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | gain                | :class:`Gain`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | importance          | :type:`Importance`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | headLocked          | :type:`HeadLocked`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | headphoneVirtualise | :class:`HeadphoneVirtualise` | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | screenEdgeLock      | :class:`ScreenEdgeLock`      | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | channelLock         | :class:`ChannelLock`         | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | objectDivergence    | :class:`ObjectDivergence`    | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | jumpPosition        | :class:`JumpPosition`        | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | screenRef           | :type:`ScreenRef`            | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   *
   * ``cartesian`` and ``position`` attributes are linked; see :func:`void
   * set(Cartesian)`, :func:`void set(Position)`, :func:`void
   * set(CartesianPosition)` and :func:`void set(SphericalPosition)`.
   *
   * \endrst
   *
   * @warning This class has unsupported parameters
   *   - ZoneExclusion
   */
  class AudioBlockFormatObjects : private detail::AudioBlockFormatObjectsBase {
   public:
    typedef AudioBlockFormatObjectsTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    explicit AudioBlockFormatObjects(CartesianPosition position,
                                     Parameters... optionalNamedArgs);
    template <typename... Parameters>
    explicit AudioBlockFormatObjects(SphericalPosition position,
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

    using detail::AudioBlockFormatObjectsBase::set;

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
    /// @brief ScreenEdgeLock setter
    ADM_EXPORT void set(ScreenEdgeLock screenEdgeLock);
    /// @brief ChannelLock setter
    ADM_EXPORT void set(ChannelLock channelLock);
    /// @brief ObjectDivergence setter
    ADM_EXPORT void set(ObjectDivergence objectDivergence);
    /// @brief JumpPosition setter
    ADM_EXPORT void set(JumpPosition jumpPosition);

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
    using detail::AudioBlockFormatObjectsBase::get;
    using detail::AudioBlockFormatObjectsBase::has;
    using detail::AudioBlockFormatObjectsBase::isDefault;
    using detail::AudioBlockFormatObjectsBase::unset;

    ADM_EXPORT Cartesian get(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT Position get(detail::ParameterTraits<Position>::tag) const;
    ADM_EXPORT SphericalPosition
        get(detail::ParameterTraits<SphericalPosition>::tag) const;
    ADM_EXPORT CartesianPosition
        get(detail::ParameterTraits<CartesianPosition>::tag) const;
    ADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;
    ADM_EXPORT ChannelLock get(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT ObjectDivergence
        get(detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT JumpPosition
        get(detail::ParameterTraits<JumpPosition>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Position>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<SphericalPosition>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<CartesianPosition>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<JumpPosition>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Cartesian>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<ChannelLock>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ObjectDivergence>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<JumpPosition>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Cartesian>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Position>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<SphericalPosition>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<CartesianPosition>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ChannelLock>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ObjectDivergence>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<JumpPosition>::tag);

    boost::optional<Cartesian> cartesian_;
    boost::optional<SphericalPosition> sphericalPosition_;
    boost::optional<CartesianPosition> cartesianPosition_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;
    boost::optional<ChannelLock> channelLock_;
    boost::optional<ObjectDivergence> objectDivergence_;
    boost::optional<JumpPosition> jumpPosition_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatObjects::AudioBlockFormatObjects(
      CartesianPosition position, Parameters... optionalNamedArgs) {
    set(position);
    detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
  }

  template <typename... Parameters>
  AudioBlockFormatObjects::AudioBlockFormatObjects(
      SphericalPosition position, Parameters... optionalNamedArgs) {
    set(position);
    detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
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
