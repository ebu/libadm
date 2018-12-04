/// @file jump_position.hpp
#pragma once

#include <boost/optional.hpp>
#include <chrono>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::JumpPositionFlag
  struct JumpPositionFlagTag {};
  /// @brief NamedType for jumpPositionFlag parameter
  using JumpPositionFlag = detail::NamedType<bool, JumpPositionFlagTag>;
  /// @brief Tag for NamedType ::InterpolationLength
  struct interpolationLengthTag {};
  /// @brief NamedType for interpolationLength parameter
  using InterpolationLength =
      detail::NamedType<std::chrono::nanoseconds, interpolationLengthTag>;

  /// @brief Tag for JumpPosition class
  struct JumpPositionTag {};
  /**
   * @brief ADM parameter class to specify a jump position
   */
  class JumpPosition {
   public:
    typedef JumpPositionTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    JumpPosition(Parameters... optionalNamedArgs);

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

    /// @brief JumpPositionFlag setter
    ADM_EXPORT void set(JumpPositionFlag objectDivergenceFlag);
    /// @brief InterpolationLength setter
    ADM_EXPORT void set(InterpolationLength interpolationLength);

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
    ADM_EXPORT JumpPositionFlag
        get(detail::ParameterTraits<JumpPositionFlag>::tag) const;
    ADM_EXPORT InterpolationLength
        get(detail::ParameterTraits<InterpolationLength>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<JumpPositionFlag>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<InterpolationLength>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<JumpPositionFlag>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<InterpolationLength>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<JumpPositionFlag>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<InterpolationLength>::tag);

    boost::optional<JumpPositionFlag> jumpPositionFlag_;
    boost::optional<InterpolationLength> interpolationLength_;

    static const JumpPositionFlag jumpPositionFlagDefault_;
    static const InterpolationLength interpolationLengthDefault_;
  };

  // ---- Free Functions ---- //

  /// @brief Helper function to check if the ::JumpPositionFlag of a
  /// JumpPosition is true
  ADM_EXPORT bool isEnabled(JumpPosition &jumpPosition);
  /// @brief Helper function to set the ::JumpPositionFlag of a JumpPosition
  /// to true
  ADM_EXPORT void enable(JumpPosition &jumpPosition);
  /// @brief Helper function to set the ::JumpPositionFlag of a JumpPosition
  /// to false
  ADM_EXPORT void disable(JumpPosition &jumpPosition);

  /// @brief Parse interpolationLength
  ADM_EXPORT InterpolationLength
  parseInterpolationLength(const std::string &length);
  /// @brief Format interpolationLength
  ADM_EXPORT std::string formatInterpolationLength(
      const InterpolationLength length);

  // ---- Implementation ---- //

  template <typename... Parameters>
  JumpPosition::JumpPosition(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter JumpPosition::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool JumpPosition::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool JumpPosition::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void JumpPosition::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
