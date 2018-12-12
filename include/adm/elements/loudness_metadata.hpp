#pragma once
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include <boost/optional.hpp>
#include <string>
#include <ostream>

namespace adm {

  /// @brief Tag for NamedType ::LoudnessMethod
  struct loudnessMethodTag {};
  /// @brief NamedType for loudnessMethod parameter
  using LoudnessMethod = detail::NamedType<std::string, loudnessMethodTag>;
  /// @brief Tag for NamedType ::LoudnessRecType
  struct loudnessRecTypeTag {};
  /// @brief NamedType for loudnessRecType parameter
  using LoudnessRecType = detail::NamedType<std::string, loudnessRecTypeTag>;
  /// @brief Tag for NamedType ::LoudnessCorrectionType
  struct loudnessCorrectionTypeTag {};
  /// @brief NamedType for loudnessCorrectionType parameter
  using LoudnessCorrectionType =
      detail::NamedType<std::string, loudnessCorrectionTypeTag>;

  /// @brief Tag for NamedType ::IntegratedLoudness
  struct integratedLoudnessTag {};
  /// @brief NamedType for integratedLoudness parameter
  using IntegratedLoudness = detail::NamedType<float, integratedLoudnessTag>;
  /// @brief Tag for NamedType ::LoudnessRange
  struct loudnessRangeTag {};
  /// @brief NamedType for loudnessRange parameter
  using LoudnessRange = detail::NamedType<float, loudnessRangeTag>;
  /// @brief Tag for NamedType ::MaxTruePeak
  struct maxTruePeakTag {};
  /// @brief NamedType for maxTruePeak parameter
  using MaxTruePeak = detail::NamedType<float, maxTruePeakTag>;
  /// @brief Tag for NamedType ::MaxMomentary
  struct maxMomentaryTag {};
  /// @brief NamedType for maxMomentary parameter
  using MaxMomentary = detail::NamedType<float, maxMomentaryTag>;
  /// @brief Tag for NamedType ::MaxShortTerm
  struct maxShortTermTag {};
  /// @brief NamedType for maxShortTerm parameter
  using MaxShortTerm = detail::NamedType<float, maxShortTermTag>;
  /// @brief Tag for NamedType ::DialogueLoudness
  struct dialogueLoudnessTag {};
  /// @brief NamedType for dialogueLoudness parameter
  using DialogueLoudness = detail::NamedType<float, dialogueLoudnessTag>;

  /// @brief Tag for LoudnessMetadata class
  struct LoudnessMetadataTag {};

  class LoudnessMetadata {
   public:
    typedef LoudnessMetadataTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    LoudnessMetadata(Parameters... optionalNamedArgs);

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

    /// @brief LoudnessMethod setter
    ADM_EXPORT void set(LoudnessMethod loudnessMethod);
    /// @brief LoudnessRecType setter
    ADM_EXPORT void set(LoudnessRecType loudnessRecType);
    /// @brief LoudnessCorrectionType setter
    ADM_EXPORT void set(LoudnessCorrectionType loudnessLoudnessCorrectionType);
    /// @brief IntegratedLoudness setter
    ADM_EXPORT void set(IntegratedLoudness integratedLoudness);
    /// @brief LoudnessRange setter
    ADM_EXPORT void set(LoudnessRange loudnessRange);
    /// @brief MaxTruePeak setter
    ADM_EXPORT void set(MaxTruePeak maxTruePeak);
    /// @brief MaxMomentary setter
    ADM_EXPORT void set(MaxMomentary maxMomentary);
    /// @brief MaxShortTerm setter
    ADM_EXPORT void set(MaxShortTerm maxShortTerm);
    /// @brief DialogueLoudness setter
    ADM_EXPORT void set(DialogueLoudness dialogueLoudness);

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
    ADM_EXPORT LoudnessMethod
        get(detail::ParameterTraits<LoudnessMethod>::tag) const;
    ADM_EXPORT LoudnessRecType
        get(detail::ParameterTraits<LoudnessRecType>::tag) const;
    ADM_EXPORT LoudnessCorrectionType
        get(detail::ParameterTraits<LoudnessCorrectionType>::tag) const;
    ADM_EXPORT IntegratedLoudness
        get(detail::ParameterTraits<IntegratedLoudness>::tag) const;
    ADM_EXPORT LoudnessRange
        get(detail::ParameterTraits<LoudnessRange>::tag) const;
    ADM_EXPORT MaxTruePeak get(detail::ParameterTraits<MaxTruePeak>::tag) const;
    ADM_EXPORT MaxMomentary
        get(detail::ParameterTraits<MaxMomentary>::tag) const;
    ADM_EXPORT MaxShortTerm
        get(detail::ParameterTraits<MaxShortTerm>::tag) const;
    ADM_EXPORT DialogueLoudness
        get(detail::ParameterTraits<DialogueLoudness>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<LoudnessMethod>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<LoudnessRecType>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<LoudnessCorrectionType>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<IntegratedLoudness>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<LoudnessRange>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<MaxTruePeak>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<MaxMomentary>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<MaxShortTerm>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DialogueLoudness>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<LoudnessMethod>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<LoudnessRecType>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<LoudnessCorrectionType>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<IntegratedLoudness>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<LoudnessRange>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<MaxTruePeak>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<MaxMomentary>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<MaxShortTerm>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DialogueLoudness>::tag);

    boost::optional<LoudnessMethod> loudnessMethod_;
    boost::optional<LoudnessRecType> loudnessRecType_;
    boost::optional<LoudnessCorrectionType> loudnessCorrectionType_;
    boost::optional<IntegratedLoudness> integratedLoudness_;
    boost::optional<LoudnessRange> loudnessRange_;
    boost::optional<MaxTruePeak> maxTruePeak_;
    boost::optional<MaxMomentary> maxMomentary_;
    boost::optional<MaxShortTerm> maxShortTerm_;
    boost::optional<DialogueLoudness> dialogueLoudness_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  LoudnessMetadata::LoudnessMetadata(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter LoudnessMetadata::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool LoudnessMetadata::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool LoudnessMetadata::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void LoudnessMetadata::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }
}  // namespace adm
