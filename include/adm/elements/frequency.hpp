/// @file frequency.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::FrequencyType
  struct FrequencyTypeTag {};
  /**
   * @brief NamedType for the frequency type (filter type)
   *
   * Valid values are "lowPass" and "highPass"
   */
  using FrequencyType = detail::NamedType<std::string, FrequencyTypeTag,
                                          detail::FrequencyTypeValidator>;
  /// @brief Tag for NamedType ::HighPass
  struct HighPassTag {};
  /**
   * @brief NamedType for the high pass frequency
   *
   * Valid values are at least 0
   */
  using HighPass =
      detail::NamedType<float, HighPassTag, detail::MinValidator<0>>;
  /// @brief Tag for NamedType ::LowPass
  struct LowPassTag {};
  /**
   * @brief NamedType for the low pass frequency
   *
   * Valid values are at least 0
   */
  using LowPass = detail::NamedType<float, LowPassTag, detail::MinValidator<0>>;

  /// @brief Tag for Frequency class
  struct FrequencyTag {};
  /**
   * @brief ADM parameter class to specify a speaker position
   */
  class Frequency {
   public:
    typedef FrequencyTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    Frequency(Parameters... optionalNamedArgs);

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

    /// @brief HighPass setter
    ADM_EXPORT void set(HighPass frequency);
    /// @brief LowPass setter
    ADM_EXPORT void set(LowPass frequency);

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
    ADM_EXPORT HighPass get(detail::ParameterTraits<HighPass>::tag) const;
    ADM_EXPORT LowPass get(detail::ParameterTraits<LowPass>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<HighPass>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<LowPass>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<HighPass>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<LowPass>::tag);

    boost::optional<HighPass> highPassFrequency_;
    boost::optional<LowPass> lowPassFrequency_;
  };

  // ---- Free functions ---- //

  /// @brief Helper function to check if Frequency is a low pass filter
  ADM_EXPORT bool isLowPass(const Frequency &frequency);
  /// @brief Helper function to check if Frequency is a high pass filter
  ADM_EXPORT bool isHighPass(const Frequency &frequency);
  /// @brief Helper function to check if Frequency is a band pass filter
  ADM_EXPORT bool isBandPass(const Frequency &frequency);

  // ---- Implementation ---- //

  template <typename... Parameters>
  Frequency::Frequency(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter Frequency::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool Frequency::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool Frequency::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void Frequency::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
