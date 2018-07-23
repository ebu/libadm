#include "adm/elements/gain_interaction_range.hpp"

namespace adm {

  // ---- Getter ---- //
  GainInteractionMin GainInteractionRange::get(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_.get();
  }
  GainInteractionMax GainInteractionRange::get(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_.get();
  }

  // ---- Has ---- //
  bool GainInteractionRange::has(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_ != boost::none;
  }
  bool GainInteractionRange::has(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_ != boost::none;
  }

  // ---- isDefault ---- //
  bool GainInteractionRange::isDefault(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_ == boost::none;
  }
  bool GainInteractionRange::isDefault(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_ == boost::none;
  }

  // ---- Setter ---- //
  void GainInteractionRange::set(GainInteractionMin gainMin) {
    gainMin_ = gainMin;
  }
  void GainInteractionRange::set(GainInteractionMax gainMax) {
    gainMax_ = gainMax;
  }

  // ---- Unsetter ---- //
  void GainInteractionRange::unset(
      detail::ParameterTraits<GainInteractionMin>::tag) {
    gainMin_ = boost::none;
  }
  void GainInteractionRange::unset(
      detail::ParameterTraits<GainInteractionMax>::tag) {
    gainMax_ = boost::none;
  }

}  // namespace adm
