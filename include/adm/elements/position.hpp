/// @file position.hpp
#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <iosfwd>
#include "adm/elements/position_types.hpp"
#include "adm/elements/screen_edge_lock.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/libadm_export.h"
#ifndef M_PI
#define M_PI 3.14159265359f
#endif

namespace adm {

  /// @brief Tag for SphericalPosition class
  struct SphericalPositionTag {};
  /**
   * @brief ADM parameter class to specify a spherical position
   */
  class SphericalPosition {
   public:
    typedef SphericalPositionTag tag;

    /// @brief Constructor without optional parameters
    LIBADM_EXPORT SphericalPosition(Azimuth azimuth = Azimuth(0.f),
                                    Elevation elevation = Elevation(0.f));
    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    SphericalPosition(Azimuth azimuth, Elevation elevation,
                      Parameters... optionalNamedArgs);

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
    /// @brief Elevation setter
    LIBADM_EXPORT void set(Elevation elevation);
    /// @brief Distance setter
    LIBADM_EXPORT void set(Distance distance);
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
    void print(std::ostream& os) const;

   private:
    LIBADM_EXPORT Azimuth get(detail::ParameterTraits<Azimuth>::tag) const;
    LIBADM_EXPORT Elevation get(detail::ParameterTraits<Elevation>::tag) const;
    LIBADM_EXPORT Distance get(detail::ParameterTraits<Distance>::tag) const;
    LIBADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    LIBADM_EXPORT bool has(detail::ParameterTraits<Azimuth>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Elevation>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Distance>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    LIBADM_EXPORT bool isDefault(detail::ParameterTraits<Distance>::tag) const;

    LIBADM_EXPORT void unset(detail::ParameterTraits<Distance>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);

    Azimuth azimuth_;
    Elevation elevation_;
    boost::optional<Distance> distance_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;

    static const Distance distanceDefault_;
  };

  /// @brief Tag for CartesianPosition class
  struct CartesianPositionTag {};
  /// @brief ADM parameter class to specify a cartesian position
  class CartesianPosition {
   public:
    typedef CartesianPositionTag tag;

    /// @brief Constructor without optional parameters
    LIBADM_EXPORT CartesianPosition(X x = X(0.f), Y y = Y(1.f));

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    CartesianPosition(X x, Y y, Parameters... optionalNamedArgs);

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
    LIBADM_EXPORT void set(X x);
    /// @brief Y setter
    LIBADM_EXPORT void set(Y y);
    /// @brief Z setter
    LIBADM_EXPORT void set(Z z);
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
    void print(std::ostream& os) const;

   private:
    LIBADM_EXPORT X get(detail::ParameterTraits<X>::tag) const;
    LIBADM_EXPORT Y get(detail::ParameterTraits<Y>::tag) const;
    LIBADM_EXPORT Z get(detail::ParameterTraits<Z>::tag) const;
    LIBADM_EXPORT ScreenEdgeLock
        get(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    LIBADM_EXPORT bool has(detail::ParameterTraits<X>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Y>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Z>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<ScreenEdgeLock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    LIBADM_EXPORT bool isDefault(detail::ParameterTraits<Z>::tag) const;

    LIBADM_EXPORT void unset(detail::ParameterTraits<Z>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<ScreenEdgeLock>::tag);

    X x_;
    Y y_;
    boost::optional<Z> z_;
    boost::optional<ScreenEdgeLock> screenEdgeLock_;

    static const Z zDefault_;
  };

  ///@brief Type to hold a SphericalPosition or CartesianPosition
  typedef boost::variant<SphericalPosition, CartesianPosition> Position;
  ADD_TRAIT(Position, PositionTag);

  // ---- Free functions ---- //

  /// @brief Helper function to check if a ::Position is spherical
  LIBADM_EXPORT bool isSpherical(const Position& position);
  /// @brief Helper function to check if a ::Position is cartesian
  LIBADM_EXPORT bool isCartesian(const Position& position);

  // ---- Implementation ---- //

  template <typename... Parameters>
  SphericalPosition::SphericalPosition(Azimuth azimuth, Elevation elevation,
                                       Parameters... optionalNamedArgs)
      : azimuth_(azimuth), elevation_(elevation) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter SphericalPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool SphericalPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool SphericalPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void SphericalPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename... Parameters>
  CartesianPosition::CartesianPosition(X x, Y y,
                                       Parameters... optionalNamedArgs)
      : x_(x), y_(y) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter CartesianPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool CartesianPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool CartesianPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void CartesianPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
