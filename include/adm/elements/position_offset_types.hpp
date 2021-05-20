#pragma once
#include "adm/detail/named_type.hpp"

namespace adm {
  /// @brief Tag for NamedType ::Azimuth
  struct AzimuthOffsetTag {};
  /**
   * @brief NamedType for the azimuth parameter of the positionOffset element
   *
   * Valid values are in the range [-180, 180]
   */
  using AzimuthOffset = detail::NamedType<float, AzimuthOffsetTag,
                                          detail::RangeValidator<-360, 360>>;
  /// @brief Tag for NamedType ::Elevation
  struct ElevationOffsetTag {};
  /**
   * @brief NamedType for the elevation parameter of the positionOffset element
   *
   * Valid values are in the range [-90, 90]
   */
  using ElevationOffset = detail::NamedType<float, ElevationOffsetTag,
                                            detail::RangeValidator<-180, 180>>;
  /// @brief Tag for NamedType ::DistanceOffset
  struct DistanceOffsetTag {};
  /**
   * @brief NamedType for the distance parameter of the positionOffset offset
   * element
   *
   * Valid values are in the range [-1, 1]
   */
  using DistanceOffset = detail::NamedType<float, DistanceOffsetTag>;
  /// @brief Tag for NamedType ::X
  struct XOffsetTag {};
  /**
   * @brief NamedType for the X parameter of the positionOffset element
   */
  using XOffset = detail::NamedType<float, XOffsetTag>;
  /// @brief Tag for NamedType ::Y
  struct YOffsetTag {};
  /**
   * @brief NamedType for the Y parameter of the positionOffset element
   */
  using YOffset = detail::NamedType<float, YOffsetTag>;
  /// @brief Tag for NamedType ::Z
  struct ZOffsetTag {};
  /**
   * @brief NamedType for the Z parameter of the positionOffset element
   */
  using ZOffset = detail::NamedType<float, ZOffsetTag>;
}  // namespace adm
