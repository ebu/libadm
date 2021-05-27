#include "adm/elements/loudness_metadata.hpp"

#include <iomanip>

namespace adm {

  namespace detail {
    template class VectorParameter<LoudnessMetadatas>;
  }

  // ---- Getter ---- //
  LoudnessMethod LoudnessMetadata::get(
      detail::ParameterTraits<LoudnessMethod>::tag) const {
    return loudnessMethod_.get();
  }
  LoudnessRecType LoudnessMetadata::get(
      detail::ParameterTraits<LoudnessRecType>::tag) const {
    return loudnessRecType_.get();
  }
  LoudnessCorrectionType LoudnessMetadata::get(
      detail::ParameterTraits<LoudnessCorrectionType>::tag) const {
    return loudnessCorrectionType_.get();
  }
  IntegratedLoudness LoudnessMetadata::get(
      detail::ParameterTraits<IntegratedLoudness>::tag) const {
    return integratedLoudness_.get();
  }
  LoudnessRange LoudnessMetadata::get(
      detail::ParameterTraits<LoudnessRange>::tag) const {
    return loudnessRange_.get();
  }
  MaxTruePeak LoudnessMetadata::get(
      detail::ParameterTraits<MaxTruePeak>::tag) const {
    return maxTruePeak_.get();
  }
  MaxMomentary LoudnessMetadata::get(
      detail::ParameterTraits<MaxMomentary>::tag) const {
    return maxMomentary_.get();
  }
  MaxShortTerm LoudnessMetadata::get(
      detail::ParameterTraits<MaxShortTerm>::tag) const {
    return maxShortTerm_.get();
  }
  DialogueLoudness LoudnessMetadata::get(
      detail::ParameterTraits<DialogueLoudness>::tag) const {
    return dialogueLoudness_.get();
  }

  // ---- Has ---- //
  bool LoudnessMetadata::has(
      detail::ParameterTraits<LoudnessMethod>::tag) const {
    return loudnessMethod_ != boost::none;
  }
  bool LoudnessMetadata::has(
      detail::ParameterTraits<LoudnessRecType>::tag) const {
    return loudnessRecType_ != boost::none;
  }
  bool LoudnessMetadata::has(
      detail::ParameterTraits<LoudnessCorrectionType>::tag) const {
    return loudnessCorrectionType_ != boost::none;
  }
  bool LoudnessMetadata::has(
      detail::ParameterTraits<IntegratedLoudness>::tag) const {
    return integratedLoudness_ != boost::none;
  }
  bool LoudnessMetadata::has(
      detail::ParameterTraits<LoudnessRange>::tag) const {
    return loudnessRange_ != boost::none;
  }
  bool LoudnessMetadata::has(detail::ParameterTraits<MaxTruePeak>::tag) const {
    return maxTruePeak_ != boost::none;
  }
  bool LoudnessMetadata::has(detail::ParameterTraits<MaxMomentary>::tag) const {
    return maxMomentary_ != boost::none;
  }
  bool LoudnessMetadata::has(detail::ParameterTraits<MaxShortTerm>::tag) const {
    return maxShortTerm_ != boost::none;
  }
  bool LoudnessMetadata::has(
      detail::ParameterTraits<DialogueLoudness>::tag) const {
    return dialogueLoudness_ != boost::none;
  }

  // ---- Setter ---- //
  void LoudnessMetadata::set(LoudnessMethod loudnessMethod) {
    loudnessMethod_ = loudnessMethod;
  }
  void LoudnessMetadata::set(LoudnessRecType loudnessRecType) {
    loudnessRecType_ = loudnessRecType;
  }
  void LoudnessMetadata::set(LoudnessCorrectionType loudnessCorrectionType) {
    loudnessCorrectionType_ = loudnessCorrectionType;
  }
  void LoudnessMetadata::set(IntegratedLoudness integratedLoudness) {
    integratedLoudness_ = integratedLoudness;
  }
  void LoudnessMetadata::set(LoudnessRange loudnessRange) {
    loudnessRange_ = loudnessRange;
  }
  void LoudnessMetadata::set(MaxTruePeak maxTruePeak) {
    maxTruePeak_ = maxTruePeak;
  }
  void LoudnessMetadata::set(MaxMomentary maxMomentary) {
    maxMomentary_ = maxMomentary;
  }
  void LoudnessMetadata::set(MaxShortTerm maxShortTerm) {
    maxShortTerm_ = maxShortTerm;
  }
  void LoudnessMetadata::set(DialogueLoudness dialogueLoudness) {
    dialogueLoudness_ = dialogueLoudness;
  }

  // ---- Unsetter ---- //
  void LoudnessMetadata::unset(detail::ParameterTraits<LoudnessMethod>::tag) {
    loudnessMethod_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<LoudnessRecType>::tag) {
    loudnessRecType_ = boost::none;
  }
  void LoudnessMetadata::unset(
      detail::ParameterTraits<LoudnessCorrectionType>::tag) {
    loudnessCorrectionType_ = boost::none;
  }
  void LoudnessMetadata::unset(
      detail::ParameterTraits<IntegratedLoudness>::tag) {
    integratedLoudness_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<LoudnessRange>::tag) {
    loudnessRange_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<MaxTruePeak>::tag) {
    maxTruePeak_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<MaxMomentary>::tag) {
    maxMomentary_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<MaxShortTerm>::tag) {
    maxShortTerm_ = boost::none;
  }
  void LoudnessMetadata::unset(detail::ParameterTraits<DialogueLoudness>::tag) {
    dialogueLoudness_ = boost::none;
  }

  // ---- Operators ---- //
  bool LoudnessMetadata::operator==(const LoudnessMetadata& other) const {
    return get<LoudnessMethod>() == other.get<LoudnessMethod>() &&
           get<LoudnessRecType>() == other.get<LoudnessRecType>() &&
           get<LoudnessCorrectionType>() == other.get<LoudnessCorrectionType>();
  }

  void LoudnessMetadata::print(std::ostream& os) const {
    os << "(";
    if (has<LoudnessMethod>()) {
      os << "loudnessMethod=" << get<LoudnessMethod>() << ", ";
    }
    if (has<LoudnessRecType>()) {
      os << "loudnessRecType=" << get<LoudnessRecType>() << ", ";
    }
    if (has<LoudnessCorrectionType>()) {
      os << "loudnessCorrectionType=" << get<LoudnessCorrectionType>() << ", ";
    }
    if (has<IntegratedLoudness>()) {
      os << "integratedLoudness=" << get<IntegratedLoudness>() << ", ";
    }
    if (has<LoudnessRange>()) {
      os << "loudnessRange=" << get<LoudnessRange>() << ", ";
    }
    if (has<MaxTruePeak>()) {
      os << "maxTruePeak=" << get<MaxTruePeak>() << ", ";
    }
    if (has<MaxMomentary>()) {
      os << "maxMomentary=" << get<MaxMomentary>() << ", ";
    }
    if (has<MaxShortTerm>()) {
      os << "maxShortTerm=" << get<MaxShortTerm>() << ", ";
    }
    if (has<DialogueLoudness>()) {
      os << "dialogueLoudness=" << get<DialogueLoudness>() << ", ";
    }
    os << ")";
  }

}  // namespace adm
