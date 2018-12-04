/// @file object_divergence.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::Divergence
  struct DivergenceTag {};
  /**
   * @brief NamedType for the divergence parameter
   *
   * Valid values are in the range [0, 1].
   */
  using Divergence =
      detail::NamedType<float, DivergenceTag, detail::RangeValidator<0, 1>>;

  /// @brief Tag for NamedType ::AzimuthRange
  struct AzimuthRangeTag {};
  /**
   * @brief NamedType for the azimuth range parameter
   *
   * Valid values are in the range [0, 180].
   */
  using AzimuthRange =
      detail::NamedType<float, AzimuthRangeTag, detail::RangeValidator<0, 180>>;

  /// @brief Tag for NamedType ::PositionRange
  struct PositionRangeTag {};
  /**
   * @brief NamedType for the position range parameter
   *
   * Valid values are in the range [0, 1].
   */
  using PositionRange =
      detail::NamedType<float, PositionRangeTag, detail::RangeValidator<0, 1>>;

  /// @brief Tag for ObjectDivergence class
  struct ObjectDivergenceTag {};
  /**
   * @brief ADM parameter class to specify the object divergence
   */
  class ObjectDivergence {
   public:
    typedef ObjectDivergenceTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    ObjectDivergence(Parameters... optionalNamedArgs);

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

    /// @brief Divergence setter
    ADM_EXPORT void set(Divergence divergence);
    /// @brief AzimuthRange setter
    ADM_EXPORT void set(AzimuthRange azimuthRange);
    /// @brief PositionRange setter
    ADM_EXPORT void set(PositionRange positionRange);

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
    ADM_EXPORT Divergence get(detail::ParameterTraits<Divergence>::tag) const;
    ADM_EXPORT AzimuthRange
        get(detail::ParameterTraits<AzimuthRange>::tag) const;
    ADM_EXPORT PositionRange
        get(detail::ParameterTraits<PositionRange>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<Divergence>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<AzimuthRange>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<PositionRange>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Divergence>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<AzimuthRange>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<PositionRange>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Divergence>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<AzimuthRange>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<PositionRange>::tag);

    boost::optional<Divergence> divergence_;
    boost::optional<AzimuthRange> azimuthRange_;
    boost::optional<PositionRange> positionRange_;

    static const Divergence divergenceDefault_;
    static const AzimuthRange azimuthRangeDefault_;
    static const PositionRange positionRangeDefault_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  ObjectDivergence::ObjectDivergence(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter ObjectDivergence::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool ObjectDivergence::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool ObjectDivergence::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void ObjectDivergence::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
