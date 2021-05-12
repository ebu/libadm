#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <iosfwd>
#include "adm/elements/position_types.hpp"
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

    /// @brief Constructor without optional parameters
    ADM_EXPORT SphericalPositionOffset(
        Azimuth azimuth = Azimuth(0.f), Elevation elevation = Elevation(0.f),
        DistanceOffset distance = DistanceOffset(0.f));

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
    ADM_EXPORT void set(Azimuth azimuth);
    /// @brief Elevation setter
    ADM_EXPORT void set(Elevation elevation);
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
    ADM_EXPORT Azimuth get(detail::ParameterTraits<Azimuth>::tag) const;
    ADM_EXPORT Elevation get(detail::ParameterTraits<Elevation>::tag) const;
    ADM_EXPORT DistanceOffset
        get(detail::ParameterTraits<DistanceOffset>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<Azimuth>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Elevation>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DistanceOffset>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    Azimuth azimuth_;
    Elevation elevation_;
    DistanceOffset distance_;
  };

  /// @brief Tag for CartesianPositionOffset class
  struct CartesianPositionOffsetTag {};
  /// @brief ADM parameter class to specify a cartesian position
  class CartesianPositionOffset {
   public:
    typedef CartesianPositionOffsetTag tag;

    /// @brief Constructor without optional parameters
    ADM_EXPORT CartesianPositionOffset(X x = X(0.f), Y y = Y(0.f),
                                       Z z = Z(0.f));

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

    /// @brief X setter
    ADM_EXPORT void set(X x);
    /// @brief Y setter
    ADM_EXPORT void set(Y y);
    /// @brief Z setter
    ADM_EXPORT void set(Z z);

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
    ADM_EXPORT X get(detail::ParameterTraits<X>::tag) const;
    ADM_EXPORT Y get(detail::ParameterTraits<Y>::tag) const;
    ADM_EXPORT Z get(detail::ParameterTraits<Z>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<X>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Y>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Z>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    X x_;
    Y y_;
    Z z_;
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
