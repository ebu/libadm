/// @file speaker_position.hpp
#pragma once

#include <vector>
#include <boost/optional.hpp>
#include "adm/detail/named_option_helper.hpp"
#include "adm/elements/screen_edge_lock.hpp"
#include "adm/elements/position_types.hpp"
#include "adm/libadm_export.h"

namespace adm {

  /// @brief Tag for NamedType ::BoundValue
  struct BoundValueTag {};
  /**
   * @brief NamedType for the bound attribute of the position element
   *
   * Valid values are "min" and "max".
   */
  using BoundValue = detail::NamedType<std::string, BoundValueTag,
                                       detail::BoundValueValidator>;

  /// @brief Tag for SpeakerPosition class
  struct SpeakerPositionTag {};
  /**
   * @brief ADM parameter class to specify a speaker position
   *
   * @warning The SpeakerPosition class does not support cartesian coordinates.
   */
  class SpeakerPosition {
   public:
    typedef SpeakerPositionTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    SpeakerPosition(Parameters... optionalNamedArgs);

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

    /// @brief Azimuth setter
    LIBADM_EXPORT void set(Azimuth azimuth);
    /// @brief AzimuthMin setter
    LIBADM_EXPORT void set(AzimuthMin azimuthMin);
    /// @brief AzimuthMax setter
    LIBADM_EXPORT void set(AzimuthMax azimuthMax);
    /// @brief Elevation setter
    LIBADM_EXPORT void set(Elevation elevation);
    /// @brief Elevation minimum setter
    LIBADM_EXPORT void set(ElevationMin elevationMin);
    /// @brief ElevationMax setter
    LIBADM_EXPORT void set(ElevationMax elevationMax);
    /// @brief Distance setter
    LIBADM_EXPORT void set(Distance distance);
    /// @brief DistanceMin setter
    LIBADM_EXPORT void set(DistanceMin distanceMin);
    /// @brief DistanceMax setter
    LIBADM_EXPORT void set(DistanceMax distanceMax);
    /// @brief ScreenEdgeLock setter
    LIBADM_EXPORT void set(ScreenEdgeLock screenEdgeLock);

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
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

   private:
    LIBADM_EXPORT Azimuth get(detail::ParameterTraits<Azimuth>::tag) const;
    LIBADM_EXPORT AzimuthMin
        get(detail::ParameterTraits<AzimuthMin>::tag) const;
    LIBADM_EXPORT AzimuthMax
        get(detail::ParameterTraits<AzimuthMax>::tag) const;
    LIBADM_EXPORT Elevation get(detail::ParameterTraits<Elevation>::tag) const;
    LIBADM_EXPORT ElevationMin
        get(detail::ParameterTraits<ElevationMin>::tag) const;
    LIBADM_EXPORT ElevationMax
        get(detail::ParameterTraits<ElevationMax>::tag) const;
    LIBADM_EXPORT Distance get(detail::ParameterTraits<Distance>::tag) const;
    LIBADM_EXPORT DistanceMin
        get(detail::ParameterTraits<DistanceMin>::tag) const;
    LIBADM_EXPORT DistanceMax
        get(detail::ParameterTraits<DistanceMax>::tag) const;
    LIBADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    LIBADM_EXPORT bool has(detail::ParameterTraits<Azimuth>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<AzimuthMin>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<AzimuthMax>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Elevation>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<ElevationMin>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<ElevationMax>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Distance>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<DistanceMin>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<DistanceMax>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    LIBADM_EXPORT void unset(detail::ParameterTraits<AzimuthMin>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<AzimuthMax>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<ElevationMin>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<ElevationMax>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Distance>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<DistanceMin>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<DistanceMax>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);

    Azimuth azimuth_;
    boost::optional<AzimuthMin> azimuthMin_;
    boost::optional<AzimuthMax> azimuthMax_;
    Elevation elevation_;
    boost::optional<ElevationMin> elevationMin_;
    boost::optional<ElevationMax> elevationMax_;
    boost::optional<Distance> distance_;
    boost::optional<DistanceMin> distanceMin_;
    boost::optional<DistanceMax> distanceMax_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  SpeakerPosition::SpeakerPosition(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter SpeakerPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool SpeakerPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool SpeakerPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void SpeakerPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
