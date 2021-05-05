#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for LabelValue
  struct LabelValueTag {};
  /// @brief NamedType for the LabelValue attribute
  using LabelValue = detail::NamedType<std::string, LabelValueTag>;

  /// @brief Tag for LabelLanguage
  struct LabelLanguageTag {};
  /// @brief NamedType for LabelLanguage attribute
  using LabelLanguage = detail::NamedType<std::string, LabelLanguageTag>;

  /// @brief Tag for LabelId
  struct LabelTag {};
  /// @brief Representation of an Label
  class Label {
   public:
    typedef LabelTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    Label(Parameters... optionalNamedArgs);

    /**
     * @brief ADM parameter getter template
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template.
     *
     * Templated has method with the ADM parameter type as template argument.
     * Returns true if the ADM parameter is set or has a default value.
     */
    template <typename Parameter>
    bool has() const;

    /// @brief set value
    ADM_EXPORT void set(LabelValue value);

    /// @brief set language
    ADM_EXPORT void set(LabelLanguage language);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    ///@{
    /**
     * @brief Operator overload
     *
     * Compares the string representation of the Label.
     */
    ADM_EXPORT bool operator==(const Label& other) const;
    ADM_EXPORT bool operator!=(const Label& other) const;
    ADM_EXPORT bool operator<(const Label& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT LabelValue get(detail::ParameterTraits<LabelValue>::tag) const;
    ADM_EXPORT LabelLanguage
        get(detail::ParameterTraits<LabelLanguage>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<LabelValue>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<LabelLanguage>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<LabelValue>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<LabelLanguage>::tag);

    boost::optional<LabelValue> value_;
    boost::optional<LabelLanguage> language_;
  };

  // ---- Implementation ---- //
  template <typename... Parameters>
  Label::Label(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter Label::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool Label::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  void Label::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  typedef Label AudioComplementaryObjectGroupLabel;
  typedef Label AudioContentLabel;
  typedef Label AudioObjectLabel;
  typedef Label AudioProgrammeLabel;
}  // namespace adm