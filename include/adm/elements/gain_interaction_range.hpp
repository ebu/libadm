/// @file gain_interaction_range.hpp
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

  struct GainInteractionMinTag {};
  /// @brief NamedType for the minimum gain parameter
  using GainInteractionMin = detail::NamedType<float, GainInteractionMinTag>;
  /// @brief Tag for NamedType ::GainInteractionMax
  struct GainInteractionMaxTag {};
  /// @brief NamedType for the maximum gain parameter
  using GainInteractionMax = detail::NamedType<float, GainInteractionMaxTag>;

  /// @brief Tag for NamedType ::GainInteractionBoundValue
  struct GainInteractionBoundValueTag {};
  /**
   * @brief NamedType for the bound attribute of the gain interaction element
   *
   * Valid values are "min" and "max".
   */
  using GainInteractionBoundValue =
      detail::NamedType<std::string, GainInteractionBoundValueTag,
                        detail::BoundValueValidator>;

  /// @brief Tag for GainInteractionRange class
  struct GainInteractionRangeTag {};
  /**
   * @brief ADM parameter class to specify a gainInteractionRange
   */
  class GainInteractionRange {
   public:
    typedef GainInteractionRangeTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order.
     */
    template <typename... Parameters>
    GainInteractionRange(Parameters... optionalNamedArgs);

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

    /// @brief GainInteractionMin setter
    ADM_EXPORT void set(GainInteractionMin);
    /// @brief GainInteractionMax setter
    ADM_EXPORT void set(GainInteractionMax);

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
    ADM_EXPORT GainInteractionMin
        get(detail::ParameterTraits<GainInteractionMin>::tag) const;
    ADM_EXPORT GainInteractionMax
        get(detail::ParameterTraits<GainInteractionMax>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<GainInteractionMin>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<GainInteractionMax>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<GainInteractionMin>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<GainInteractionMax>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<GainInteractionMin>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<GainInteractionMax>::tag);

    boost::optional<GainInteractionMin> gainMin_;
    boost::optional<GainInteractionMax> gainMax_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  GainInteractionRange::GainInteractionRange(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter GainInteractionRange::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool GainInteractionRange::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool GainInteractionRange::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void GainInteractionRange::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
