/// @file position_types.hpp
#pragma once
#include "adm/detail/named_type.hpp"

namespace adm {

  /// @brief Tag for NamedType ::Azimuth
  struct AzimuthTag {};
  /**
   * @brief NamedType for the azimuth parameter of the position element
   *
   * Valid values are in the range [-180, 180]
   */
  using Azimuth =
      detail::NamedType<float, AzimuthTag, detail::RangeValidator<-180, 180>>;
  struct AzimuthMinTag {};
  /**
   * @brief NamedType for the minimum azimuth parameter of the position
   * element
   *
   * Valid values are in the range [-180, 180]
   */
  using AzimuthMin = detail::NamedType<float, AzimuthMinTag,
                                       detail::RangeValidator<-180, 180>>;
  /// @brief Tag for NamedType ::AzimuthMax
  struct AzimuthMaxTag {};
  /**
   * @brief NamedType for the minimum azimuth parameter of the position
   * element
   *
   * Valid values are in the range [-180, 180]
   */
  using AzimuthMax = detail::NamedType<float, AzimuthMaxTag,
                                       detail::RangeValidator<-180, 180>>;
  /// @brief Tag for NamedType ::Elevation
  struct ElevationTag {};
  /**
   * @brief NamedType for the elevation parameter of the position element
   *
   * Valid values are in the range [-90, 90]
   */
  using Elevation =
      detail::NamedType<float, ElevationTag, detail::RangeValidator<-90, 90>>;
  /// @brief Tag for NamedType ::ElevationMin
  struct ElevationMinTag {};
  /**
   * @brief NamedType for the minimum elevation parameter of the position
   * element
   *
   * Valid values are in the range [-90, 90]
   */
  using ElevationMin = detail::NamedType<float, ElevationMinTag,
                                         detail::RangeValidator<-90, 90>>;
  /// @brief Tag for NamedType ::ElevationMax
  struct ElevationMaxTag {};
  /**
   * @brief NamedType for the maximum elevation parameter of the position
   * element
   *
   * Valid values are in the range [-90, 90]
   */
  using ElevationMax = detail::NamedType<float, ElevationMaxTag,
                                         detail::RangeValidator<-90, 90>>;
  /// @brief Tag for NamedType ::Distance
  struct DistanceTag {};
  /**
   * @brief NamedType for the maximum elevation parameter of the position
   * element
   *
   * Valid values are in the range [-90, 90]
   */
  using Distance =
      detail::NamedType<float, DistanceTag, detail::MinValidator<0>>;
  /// @brief Tag for NamedType ::DistanceMin
  struct DistanceMinTag {};
  /**
   * @brief NamedType for the maximum elevation parameter of the position
   * element
   *
   * Valid values are in the range [-90, 90]
   */
  using DistanceMin =
      detail::NamedType<float, DistanceMinTag, detail::RangeValidator<0, 1>>;
  /// @brief Tag for NamedType ::DistanceMax
  struct DistanceMaxTag {};
  /**
   * @brief NamedType for the maximum elevation parameter of the position
   * element
   *
   * Valid values are in the range [-90, 90]
   */
  using DistanceMax =
      detail::NamedType<float, DistanceMaxTag, detail::RangeValidator<0, 1>>;
  /// @brief Tag for NamedType ::X
  struct XTag {};
  /**
   * @brief NamedType for the X parameter of the position element
   */
  using X = detail::NamedType<float, XTag>;
  /// @brief Tag for NamedType ::XMin
  struct XMinTag {};
  /**
   * @brief NamedType for the minimum X parameter of the position
   * element
   */
  using XMin = detail::NamedType<float, XMinTag>;
  /// @brief Tag for NamedType ::XMax
  struct XMaxTag {};
  /**
   * @brief NamedType for the maximum X parameter of the position
   * element
   */
  using XMax = detail::NamedType<float, XMaxTag>;
  /// @brief Tag for NamedType ::Y
  struct YTag {};
  /**
   * @brief NamedType for the Y parameter of the position element
   */
  using Y = detail::NamedType<float, YTag>;
  /// @brief Tag for NamedType ::YMin
  struct YMinTag {};
  /**
   * @brief NamedType for the minimum Y parameter of the position
   * element
   */
  using YMin = detail::NamedType<float, YMinTag>;
  /// @brief Tag for NamedType ::YMax
  struct YMaxTag {};
  /**
   * @brief NamedType for the maximum Y parameter of the position
   * element
   */
  using YMax = detail::NamedType<float, YMaxTag>;
  /// @brief Tag for NamedType ::Z
  struct ZTag {};
  /**
   * @brief NamedType for the Z parameter of the position element
   */
  using Z = detail::NamedType<float, ZTag>;
  /// @brief Tag for NamedType ::ZMin
  struct ZMinTag {};
  /**
   * @brief NamedType for the minimum Z parameter of the position
   * element
   */
  using ZMin = detail::NamedType<float, ZMinTag>;
  /// @brief Tag for NamedType ::ZMax
  struct ZMaxTag {};
  /**
   * @brief NamedType for the maximum Z parameter of the position
   * element
   */
  using ZMax = detail::NamedType<float, ZMaxTag>;

  /// @brief Tag for NamedType ::SphericalCoordinateValue
  struct SphericalCoordinateValueTag {};
  /**
   * @brief NamedType for the SphericalCoordinateValue parameter of the
   * position element
   *
   * Valid values are "azimuth", "elevation" and "distance".
   */
  using SphericalCoordinateValue =
      detail::NamedType<std::string, SphericalCoordinateValueTag,
                        detail::SphericalCoordinateValueValidator>;
  /// @brief Tag for NamedType ::CartesianCoordinateValue
  struct CartesianCoordinateValueTag {};
  /**
   * @brief NamedType for the CartesianCoordinateValue parameter of the
   * position element
   *
   * Valid values are "X", "Y" and "Z".
   */
  using CartesianCoordinateValue =
      detail::NamedType<std::string, CartesianCoordinateValueTag,
                        detail::CartesianCoordinateValueValidator>;
}  // namespace adm
