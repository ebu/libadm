#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <iosfwd>
#include "adm/elements/position_types.hpp"
#include "adm/elements/position_offset_types.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for SphericalPositionOffset class
  struct SphericalPositionOffsetTag {};
  /**
   * @brief ADM parameter class to specify a spherical position
   */
  class SphericalPositionOffset {
   public:
    typedef SphericalPositionOffsetTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    SphericalPositionOffset(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AzimuthOffset azimuth);
    /// @brief Elevation setter
    ADM_EXPORT void set(ElevationOffset elevation);
    /// @brief DistanceOffset setter
    ADM_EXPORT void set(DistanceOffset distance);

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
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT AzimuthOffset
        get(detail::ParameterTraits<AzimuthOffset>::tag) const;
    ADM_EXPORT ElevationOffset
        get(detail::ParameterTraits<ElevationOffset>::tag) const;
    ADM_EXPORT DistanceOffset
        get(detail::ParameterTraits<DistanceOffset>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AzimuthOffset>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ElevationOffset>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DistanceOffset>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AzimuthOffset>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ElevationOffset>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<DistanceOffset>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthOffset>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ElevationOffset>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DistanceOffset>::tag);

    boost::optional<AzimuthOffset> azimuth_;
    boost::optional<ElevationOffset> elevation_;
    boost::optional<DistanceOffset> distance_;

    static const AzimuthOffset azimuthDefault_;
    static const ElevationOffset elevationDefault_;
    static const DistanceOffset distanceDefault_;
  };

  /// @brief Tag for CartesianPositionOffset class
  struct CartesianPositionOffsetTag {};
  /// @brief ADM parameter class to specify a cartesian position
  class CartesianPositionOffset {
   public:
    typedef CartesianPositionOffsetTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    CartesianPositionOffset(Parameters... optionalNamedArgs);

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

    /// @brief XOffset setter
    ADM_EXPORT void set(XOffset x);
    /// @brief YOffset setter
    ADM_EXPORT void set(YOffset y);
    /// @brief ZOffset setter
    ADM_EXPORT void set(ZOffset z);

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
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT XOffset get(detail::ParameterTraits<XOffset>::tag) const;
    ADM_EXPORT YOffset get(detail::ParameterTraits<YOffset>::tag) const;
    ADM_EXPORT ZOffset get(detail::ParameterTraits<ZOffset>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<XOffset>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<YOffset>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ZOffset>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(detail::ParameterTraits<XOffset>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<YOffset>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<ZOffset>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<XOffset>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<YOffset>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ZOffset>::tag);

    boost::optional<XOffset> x_;
    boost::optional<YOffset> y_;
    boost::optional<ZOffset> z_;

    static const XOffset xDefault_;
    static const YOffset yDefault_;
    static const ZOffset zDefault_;
  };

  ///@brief Type to hold a SphericalPositionOffset or CartesianPositionOffset
  typedef boost::variant<SphericalPositionOffset, CartesianPositionOffset>
      PositionOffset;
  ADD_TRAIT(PositionOffset, PositionOffsetTag);

  // ---- Free functions ---- //

  /// @brief Helper function to check if a ::PositionOffset is spherical
  ADM_EXPORT bool isSpherical(const PositionOffset& position);
  /// @brief Helper function to check if a ::PositionOffset is cartesian
  ADM_EXPORT bool isCartesian(const PositionOffset& position);

  // ---- Implementation ---- //
  template <typename... Parameters>
  SphericalPositionOffset::SphericalPositionOffset(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter SphericalPositionOffset::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool SphericalPositionOffset::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool SphericalPositionOffset::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void SphericalPositionOffset::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename... Parameters>
  CartesianPositionOffset::CartesianPositionOffset(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter CartesianPositionOffset::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool CartesianPositionOffset::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool CartesianPositionOffset::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void CartesianPositionOffset::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
