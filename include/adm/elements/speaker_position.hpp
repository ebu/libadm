/// @file speaker_position.hpp
#pragma once

#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include "adm/detail/named_option_helper.hpp"
#include "adm/elements/screen_edge_lock.hpp"
#include "adm/elements/position_types.hpp"
#include "adm/export.h"

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

  /// @brief Tag for CartesianSpeakerPosition class
  struct CartesianSpeakerPositionTag {};
  /**
   * @brief ADM parameter class to specify a cartesian speaker position
   */
  class CartesianSpeakerPosition {
   public:
    typedef CartesianSpeakerPositionTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    explicit CartesianSpeakerPosition(Parameters... optionalNamedArgs);

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

    /// @brief X setter
    ADM_EXPORT void set(X x);
    /// @brief XMin setter
    ADM_EXPORT void set(XMin xMin);
    /// @brief XMax setter
    ADM_EXPORT void set(XMax xMax);
    /// @brief Y setter
    ADM_EXPORT void set(Y y);
    /// @brief Y minimum setter
    ADM_EXPORT void set(YMin yMin);
    /// @brief YMax setter
    ADM_EXPORT void set(YMax yMax);
    /// @brief Z setter
    ADM_EXPORT void set(Z z);
    /// @brief ZMin setter
    ADM_EXPORT void set(ZMin zMin);
    /// @brief ZMax setter
    ADM_EXPORT void set(ZMax zMax);
    /// @brief ScreenEdgeLock setter
    ADM_EXPORT void set(ScreenEdgeLock screenEdgeLock);

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
    ADM_EXPORT X get(detail::ParameterTraits<X>::tag) const;
    ADM_EXPORT XMin get(detail::ParameterTraits<XMin>::tag) const;
    ADM_EXPORT XMax get(detail::ParameterTraits<XMax>::tag) const;
    ADM_EXPORT Y get(detail::ParameterTraits<Y>::tag) const;
    ADM_EXPORT YMin get(detail::ParameterTraits<YMin>::tag) const;
    ADM_EXPORT YMax get(detail::ParameterTraits<YMax>::tag) const;
    ADM_EXPORT Z get(detail::ParameterTraits<Z>::tag) const;
    ADM_EXPORT ZMin get(detail::ParameterTraits<ZMin>::tag) const;
    ADM_EXPORT ZMax get(detail::ParameterTraits<ZMax>::tag) const;
    ADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<X>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<XMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<XMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Y>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<YMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<YMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Z>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ZMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ZMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Z>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<XMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<XMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<YMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<YMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Z>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ZMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ZMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);

    X x_;
    boost::optional<XMin> xMin_;
    boost::optional<XMax> xMax_;
    Y y_;
    boost::optional<YMin> yMin_;
    boost::optional<YMax> yMax_;
    boost::optional<Z> z_;
    boost::optional<ZMin> zMin_;
    boost::optional<ZMax> zMax_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  CartesianSpeakerPosition::CartesianSpeakerPosition(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter CartesianSpeakerPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool CartesianSpeakerPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool CartesianSpeakerPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void CartesianSpeakerPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  struct SphericalSpeakerPositionTag {};
  /**
   * @brief ADM parameter class to specify a spherical speaker position
   */
  class SphericalSpeakerPosition {
   public:
    typedef SphericalSpeakerPositionTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    explicit SphericalSpeakerPosition(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(Azimuth azimuth);
    /// @brief AzimuthMin setter
    ADM_EXPORT void set(AzimuthMin azimuthMin);
    /// @brief AzimuthMax setter
    ADM_EXPORT void set(AzimuthMax azimuthMax);
    /// @brief Elevation setter
    ADM_EXPORT void set(Elevation elevation);
    /// @brief Elevation minimum setter
    ADM_EXPORT void set(ElevationMin elevationMin);
    /// @brief ElevationMax setter
    ADM_EXPORT void set(ElevationMax elevationMax);
    /// @brief Distance setter
    ADM_EXPORT void set(Distance distance);
    /// @brief DistanceMin setter
    ADM_EXPORT void set(DistanceMin distanceMin);
    /// @brief DistanceMax setter
    ADM_EXPORT void set(DistanceMax distanceMax);
    /// @brief ScreenEdgeLock setter
    ADM_EXPORT void set(ScreenEdgeLock screenEdgeLock);

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
    ADM_EXPORT Azimuth get(detail::ParameterTraits<Azimuth>::tag) const;
    ADM_EXPORT AzimuthMin get(detail::ParameterTraits<AzimuthMin>::tag) const;
    ADM_EXPORT AzimuthMax get(detail::ParameterTraits<AzimuthMax>::tag) const;
    ADM_EXPORT Elevation get(detail::ParameterTraits<Elevation>::tag) const;
    ADM_EXPORT ElevationMin
        get(detail::ParameterTraits<ElevationMin>::tag) const;
    ADM_EXPORT ElevationMax
        get(detail::ParameterTraits<ElevationMax>::tag) const;
    ADM_EXPORT Distance get(detail::ParameterTraits<Distance>::tag) const;
    ADM_EXPORT DistanceMin get(detail::ParameterTraits<DistanceMin>::tag) const;
    ADM_EXPORT DistanceMax get(detail::ParameterTraits<DistanceMax>::tag) const;
    ADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<Azimuth>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<AzimuthMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<AzimuthMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Elevation>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ElevationMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ElevationMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Distance>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DistanceMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DistanceMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(detail::ParameterTraits<Distance>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ElevationMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ElevationMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Distance>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DistanceMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DistanceMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);

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
  SphericalSpeakerPosition::SphericalSpeakerPosition(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter SphericalSpeakerPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool SphericalSpeakerPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool SphericalSpeakerPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void SphericalSpeakerPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
