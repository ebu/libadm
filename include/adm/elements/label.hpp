#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /**
   * Helper to deduce the correct `IteratorRange` type for
   * `const_iterators` to the requested `LabelType` type
   * @headerfile label.hpp <label.hpp>
   */
  template <typename LabelType>
  using LabelConstRange =
      boost::iterator_range<typename std::vector<LabelType>::const_iterator>;

  /**
   * Helper  to deduce the correct `IteratorRange` type for
   * `iterators` to the requested `LabelType` type
   * @headerfile label.hpp <label.hpp>
   */
  template <typename LabelType>
  using LabelRange =
      boost::iterator_range<typename std::vector<LabelType>::iterator>;

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
  template <class TagType>
  class Label {
   public:
    typedef TagType tag;

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
    void set(LabelValue value);

    /// @brief set language
    void set(LabelLanguage language);

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
    template <typename OtherTagType>
    bool operator==(const Label<OtherTagType>& other) const;
    template <typename OtherTagType>
    bool operator!=(const Label<OtherTagType>& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    LabelValue get(detail::ParameterTraits<LabelValue>::tag) const;
    LabelLanguage get(detail::ParameterTraits<LabelLanguage>::tag) const;

    bool has(detail::ParameterTraits<LabelValue>::tag) const;
    bool has(detail::ParameterTraits<LabelLanguage>::tag) const;

    void unset(detail::ParameterTraits<LabelLanguage>::tag);

    LabelValue value_{""};
    boost::optional<LabelLanguage> language_;
  };

  // ---- Implementation ---- //
  template <class TagType>
  template <typename... Parameters>
  Label<TagType>::Label(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <class TagType>
  template <typename Parameter>
  Parameter Label<TagType>::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <class TagType>
  template <typename Parameter>
  bool Label<TagType>::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <class TagType>
  template <typename Parameter>
  void Label<TagType>::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <class TagType>
  void Label<TagType>::print(std::ostream& os) const {
      os << "value=" << get<LabelValue>().get();
      if(has<LabelLanguage>()) {
          os << ", lang=" << get<LabelLanguage>();
      }
  }

  /// ---- Getter ---- //
  template <class TagType>
  LabelLanguage Label<TagType>::get(
      detail::ParameterTraits<LabelLanguage>::tag) const {
    return language_.get();
  }
  template <class TagType>
  LabelValue Label<TagType>::get(
      detail::ParameterTraits<LabelValue>::tag) const {
    return value_;
  }

  /// ---- Has ---- //
  template <class TagType>
  bool Label<TagType>::has(detail::ParameterTraits<LabelLanguage>::tag) const {
    return language_ != boost::none;
  }
  template <class TagType>
  bool Label<TagType>::has(detail::ParameterTraits<LabelValue>::tag) const {
    return true;
  }

  /// ---- Setter ---- //
  template <class TagType>
  void Label<TagType>::set(LabelValue value) {
    value_ = value;
  }
  template <class TagType>
  void Label<TagType>::set(LabelLanguage language) {
    language_ = language;
  }

  /// ---- Unsetter ---- //
  template <class TagType>
  void Label<TagType>::unset(detail::ParameterTraits<LabelLanguage>::tag) {
    language_ = boost::none;
  }

  /// ---- Operators ---- //
  template <class TagType>
  template <typename OtherTagType>
  bool Label<TagType>::operator==(const Label<OtherTagType>& other) const {
    return get<LabelValue>() == other.template get<LabelValue>() &&
           get<LabelLanguage>() == other.template get<LabelLanguage>();
  }
  template <class TagType>
  template <typename OtherTagType>
  bool Label<TagType>::operator!=(const Label<OtherTagType>& other) const {
    return !(*this == other);
  }

  // Label Typedefs

  struct AudioComplementaryObjectGroupLabelTag {};
  struct AudioContentLabelTag {};
  struct AudioObjectLabelTag {};
  struct AudioProgrammeLabelTag {};

  typedef Label<AudioComplementaryObjectGroupLabelTag>
      AudioComplementaryObjectGroupLabel;
  typedef Label<AudioContentLabelTag> AudioContentLabel;
  typedef Label<AudioObjectLabelTag> AudioObjectLabel;
  typedef Label<AudioProgrammeLabelTag> AudioProgrammeLabel;

}  // namespace adm