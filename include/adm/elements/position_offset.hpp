#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <iosfwd>
#include "adm/elements/position_types.hpp"
#include "adm/elements/position_offset_types.hpp"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"

namespace adm {

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<AzimuthOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<ElevationOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<DistanceOffset>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<XOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<YOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<ZOffset>;

    using SphericalPositionOffsetBase =
        HasParameters<OptionalParameter<AzimuthOffset>,
                      OptionalParameter<ElevationOffset>,
                      OptionalParameter<DistanceOffset>>;

    using CartesianPositionOffsetBase =
        HasParameters<OptionalParameter<XOffset>, OptionalParameter<YOffset>,
                      OptionalParameter<ZOffset>>;

  }  // namespace detail

  /// @brief Tag for SphericalPositionOffset class
  struct SphericalPositionOffsetTag {};

  /// @brief ADM parameter class to specify a spherical position offset
  ///
  /// Supported parameters are as follows:
  ///
  /// \rst
  /// +----------------------------------------+-------------------------+----------------------------+
  /// | ADM Parameter                          | Parameter Type          | Pattern Type               |
  /// +========================================+=========================+============================+
  /// | positionOffset, coordinate="azimuth"   | :type:`AzimuthOffset`   | :class:`RequiredParameter` |
  /// +----------------------------------------+-------------------------+----------------------------+
  /// | positionOffset, coordinate="elevation" | :type:`ElevationOffset` | :class:`RequiredParameter` |
  /// +----------------------------------------+-------------------------+----------------------------+
  /// | positionOffset, coordinate="distance"  | :type:`DistanceOffset`  | :class:`RequiredParameter` |
  /// +----------------------------------------+-------------------------+----------------------------+
  /// \endrst
  class SphericalPositionOffset
      : private detail::SphericalPositionOffsetBase,
        private detail::AddWrapperMethods<SphericalPositionOffset> {
   public:
    typedef SphericalPositionOffsetTag tag;

    /// @brief Constructor template
    ///
    /// Templated constructor which accepts a variable number of ADM parameters
    /// in random order after the mandatory ADM parameters.
    template <typename... Parameters>
    explicit SphericalPositionOffset(Parameters&&... optionalNamedArgs);

    /// @brief Print overview to ostream
    ADM_EXPORT void print(std::ostream& os) const;

    using detail::SphericalPositionOffsetBase::set;
    using detail::AddWrapperMethods<SphericalPositionOffset>::get;
    using detail::AddWrapperMethods<SphericalPositionOffset>::has;
    using detail::AddWrapperMethods<SphericalPositionOffset>::isDefault;
    using detail::AddWrapperMethods<SphericalPositionOffset>::unset;

   private:
    using detail::SphericalPositionOffsetBase::get;
    using detail::SphericalPositionOffsetBase::has;
    using detail::SphericalPositionOffsetBase::isDefault;
    using detail::SphericalPositionOffsetBase::unset;

    friend class detail::AddWrapperMethods<SphericalPositionOffset>;
  };

  /// @brief Tag for CartesianPositionOffset class
  struct CartesianPositionOffsetTag {};

  /// @brief ADM parameter class to specify a cartesian position offset
  ///
  /// Supported parameters are as follows:
  ///
  /// \rst
  /// +--------------------------------+-----------------+----------------------------+
  /// | ADM Parameter                  | Parameter Type  | Pattern Type               |
  /// +================================+=================+============================+
  /// | positionOffset, coordinate="X" | :type:`XOffset` | :class:`RequiredParameter` |
  /// +--------------------------------+-----------------+----------------------------+
  /// | positionOffset, coordinate="Y" | :type:`YOffset` | :class:`RequiredParameter` |
  /// +--------------------------------+-----------------+----------------------------+
  /// | positionOffset, coordinate="Z" | :type:`ZOffset` | :class:`RequiredParameter` |
  /// +--------------------------------+-----------------+----------------------------+
  /// \endrst
  class CartesianPositionOffset
      : private detail::CartesianPositionOffsetBase,
        private detail::AddWrapperMethods<CartesianPositionOffset> {
   public:
    typedef CartesianPositionOffsetTag tag;

    /// @brief Constructor template
    ///
    /// Templated constructor which accepts a variable number of ADM parameters
    /// in random order after the mandatory ADM parameters.
    template <typename... Parameters>
    explicit CartesianPositionOffset(Parameters&&... optionalNamedArgs);

    /// @brief Print overview to ostream
    ADM_EXPORT void print(std::ostream& os) const;

    using detail::CartesianPositionOffsetBase::set;
    using detail::AddWrapperMethods<CartesianPositionOffset>::get;
    using detail::AddWrapperMethods<CartesianPositionOffset>::has;
    using detail::AddWrapperMethods<CartesianPositionOffset>::isDefault;
    using detail::AddWrapperMethods<CartesianPositionOffset>::unset;

   private:
    using detail::CartesianPositionOffsetBase::get;
    using detail::CartesianPositionOffsetBase::has;
    using detail::CartesianPositionOffsetBase::isDefault;
    using detail::CartesianPositionOffsetBase::unset;

    friend class detail::AddWrapperMethods<CartesianPositionOffset>;
  };

  /// @brief Type to hold a SphericalPositionOffset or CartesianPositionOffset
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
      Parameters&&... optionalNamedArgs) {
    detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
  }

  template <typename... Parameters>
  CartesianPositionOffset::CartesianPositionOffset(
      Parameters&&... optionalNamedArgs) {
    detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
  }

}  // namespace adm
