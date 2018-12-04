/// @file position_interaction_range.hpp
#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <iosfwd>
#include "adm/elements/position_types.hpp"
#include "adm/elements/screen_edge_lock.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
#ifndef M_PI
#define M_PI 3.14159265359f
#endif

namespace adm {

  struct AzimuthInteractionMinTag {};
  /// @brief NamedType for the minimum azimuth interaction parameter
  using AzimuthInteractionMin =
      detail::NamedType<float, AzimuthInteractionMinTag>;
  /// @brief Tag for NamedType ::AzimuthInteractionMax
  struct AzimuthInteractionMaxTag {};
  /// @brief NamedType for the maximum azimuth interaction parameter
  using AzimuthInteractionMax =
      detail::NamedType<float, AzimuthInteractionMaxTag>;

  struct ElevationInteractionMinTag {};
  /// @brief NamedType for the minimum elevation interaction parameter
  using ElevationInteractionMin =
      detail::NamedType<float, ElevationInteractionMinTag>;
  /// @brief Tag for NamedType ::ElevationInteractionMax
  struct ElevationInteractionMaxTag {};
  /// @brief NamedType for the maximum elevation interaction parameter
  using ElevationInteractionMax =
      detail::NamedType<float, ElevationInteractionMaxTag>;

  struct DistanceInteractionMinTag {};
  /// @brief NamedType for the minimum distance interaction parameter
  using DistanceInteractionMin =
      detail::NamedType<float, DistanceInteractionMinTag>;
  /// @brief Tag for NamedType ::DistanceInteractionMax
  struct DistanceInteractionMaxTag {};
  /// @brief NamedType for the maximum distance interaction parameter
  using DistanceInteractionMax =
      detail::NamedType<float, DistanceInteractionMaxTag>;

  struct XInteractionMinTag {};
  /// @brief NamedType for the minimum x interaction parameter
  using XInteractionMin = detail::NamedType<float, XInteractionMinTag>;
  /// @brief Tag for NamedType ::XInteractionMax
  struct XInteractionMaxTag {};
  /// @brief NamedType for the maximum x interaction parameter
  using XInteractionMax = detail::NamedType<float, XInteractionMaxTag>;

  struct YInteractionMinTag {};
  /// @brief NamedType for the minimum y interaction parameter
  using YInteractionMin = detail::NamedType<float, YInteractionMinTag>;
  /// @brief Tag for NamedType ::YInteractionMax
  struct YInteractionMaxTag {};
  /// @brief NamedType for the maximum y interaction parameter
  using YInteractionMax = detail::NamedType<float, YInteractionMaxTag>;

  struct ZInteractionMinTag {};
  /// @brief NamedType for the minimum z interaction parameter
  using ZInteractionMin = detail::NamedType<float, ZInteractionMinTag>;
  /// @brief Tag for NamedType ::ZInteractionMax
  struct ZInteractionMaxTag {};
  /// @brief NamedType for the maximum z interaction parameter
  using ZInteractionMax = detail::NamedType<float, ZInteractionMaxTag>;

  /// @brief Tag for NamedType ::PositionInteractionBoundValue
  struct PositionInteractionBoundValueTag {};
  /**
   * @brief NamedType for the bound attribute of the position element
   *
   * Valid values are "min" and "max".
   */
  using PositionInteractionBoundValue =
      detail::NamedType<std::string, PositionInteractionBoundValueTag,
                        detail::BoundValueValidator>;

  /// @brief Tag for NamedType ::CoordinateInteractionValue
  struct CoordinateInteractionValueTag {};
  /**
   * @brief NamedType for the CoordinateInteractionValue parameter of the
   * position element
   *
   * Valid values are "azimuth", "elevation", "distance", "X", "Y" and "Z".
   */
  using CoordinateInteractionValue =
      detail::NamedType<std::string, CoordinateInteractionValueTag,
                        detail::CoordinateValueValidator>;
  /// @brief Tag for PositionInteractionRange class
  struct PositionInteractionRangeTag {};
  /**
   * @brief ADM parameter class to specify a positionInteractionRange
   */
  class PositionInteractionRange {
   public:
    typedef PositionInteractionRangeTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order.
     */
    template <typename... Parameters>
    PositionInteractionRange(Parameters... optionalNamedArgs);

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

    /// @brief AzimuthInteractionMin setter
    ADM_EXPORT void set(AzimuthInteractionMin);
    /// @brief AzimuthInteractionMax setter
    ADM_EXPORT void set(AzimuthInteractionMax);
    /// @brief ElevationInteractionMin setter
    ADM_EXPORT void set(ElevationInteractionMin);
    /// @brief ElevationInteractionMax setter
    ADM_EXPORT void set(ElevationInteractionMax);
    /// @brief DistanceInteractionMin setter
    ADM_EXPORT void set(DistanceInteractionMin);
    /// @brief DistanceInteractionMax setter
    ADM_EXPORT void set(DistanceInteractionMax);
    /// @brief XInteractionMin setter
    ADM_EXPORT void set(XInteractionMin);
    /// @brief XInteractionMax setter
    ADM_EXPORT void set(XInteractionMax);
    /// @brief YInteractionMin setter
    ADM_EXPORT void set(YInteractionMin);
    /// @brief YInteractionMax setter
    ADM_EXPORT void set(YInteractionMax);
    /// @brief ZInteractionMin setter
    ADM_EXPORT void set(ZInteractionMin);
    /// @brief ZInteractionMax setter
    ADM_EXPORT void set(ZInteractionMax);

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
    ADM_EXPORT AzimuthInteractionMin
        get(detail::ParameterTraits<AzimuthInteractionMin>::tag) const;
    ADM_EXPORT AzimuthInteractionMax
        get(detail::ParameterTraits<AzimuthInteractionMax>::tag) const;
    ADM_EXPORT ElevationInteractionMin
        get(detail::ParameterTraits<ElevationInteractionMin>::tag) const;
    ADM_EXPORT ElevationInteractionMax
        get(detail::ParameterTraits<ElevationInteractionMax>::tag) const;
    ADM_EXPORT DistanceInteractionMin
        get(detail::ParameterTraits<DistanceInteractionMin>::tag) const;
    ADM_EXPORT DistanceInteractionMax
        get(detail::ParameterTraits<DistanceInteractionMax>::tag) const;
    ADM_EXPORT XInteractionMin
        get(detail::ParameterTraits<XInteractionMin>::tag) const;
    ADM_EXPORT XInteractionMax
        get(detail::ParameterTraits<XInteractionMax>::tag) const;
    ADM_EXPORT YInteractionMin
        get(detail::ParameterTraits<YInteractionMin>::tag) const;
    ADM_EXPORT YInteractionMax
        get(detail::ParameterTraits<YInteractionMax>::tag) const;
    ADM_EXPORT ZInteractionMin
        get(detail::ParameterTraits<ZInteractionMin>::tag) const;
    ADM_EXPORT ZInteractionMax
        get(detail::ParameterTraits<ZInteractionMax>::tag) const;

    ADM_EXPORT bool has(
        detail::ParameterTraits<AzimuthInteractionMin>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AzimuthInteractionMax>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<ElevationInteractionMin>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<ElevationInteractionMax>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<DistanceInteractionMin>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<DistanceInteractionMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<XInteractionMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<XInteractionMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<YInteractionMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<YInteractionMax>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ZInteractionMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ZInteractionMax>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AzimuthInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AzimuthInteractionMax>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ElevationInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ElevationInteractionMax>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<DistanceInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<DistanceInteractionMax>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<XInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<XInteractionMax>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<YInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<YInteractionMax>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ZInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ZInteractionMax>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthInteractionMax>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<ElevationInteractionMin>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<ElevationInteractionMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DistanceInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DistanceInteractionMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<XInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<XInteractionMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<YInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<YInteractionMax>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ZInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ZInteractionMax>::tag);

    boost::optional<AzimuthInteractionMin> azimuthMin_;
    boost::optional<AzimuthInteractionMax> azimuthMax_;
    boost::optional<ElevationInteractionMin> elevationMin_;
    boost::optional<ElevationInteractionMax> elevationMax_;
    boost::optional<DistanceInteractionMin> distanceMin_;
    boost::optional<DistanceInteractionMax> distanceMax_;
    boost::optional<XInteractionMin> xMin_;
    boost::optional<XInteractionMax> xMax_;
    boost::optional<YInteractionMin> yMin_;
    boost::optional<YInteractionMax> yMax_;
    boost::optional<ZInteractionMin> zMin_;
    boost::optional<ZInteractionMax> zMax_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  PositionInteractionRange::PositionInteractionRange(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter PositionInteractionRange::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool PositionInteractionRange::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool PositionInteractionRange::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void PositionInteractionRange::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
