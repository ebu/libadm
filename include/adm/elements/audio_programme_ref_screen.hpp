#pragma once
#include <ostream>
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/elements/coordinate_mode.hpp"
#include "adm/export.h"

namespace adm {

  struct AudioProgrammeReferenceScreenTag {};

  namespace detail {
    using AudioProgrammeReferenceScreenBase =
        HasParameters<OptionalParameter<CoordinateMode>>;
  }  // namespace detail

  class AudioProgrammeReferenceScreen
      : private detail::AudioProgrammeReferenceScreenBase,
        private detail::AddWrapperMethods<AudioProgrammeReferenceScreen> {
   public:
    using tag = AudioProgrammeReferenceScreenTag;

    template <typename... Parameters>
    explicit AudioProgrammeReferenceScreen(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::AudioProgrammeReferenceScreenBase::set;
    using detail::AddWrapperMethods<AudioProgrammeReferenceScreen>::get;
    using detail::AddWrapperMethods<AudioProgrammeReferenceScreen>::has;
    using detail::AddWrapperMethods<AudioProgrammeReferenceScreen>::isDefault;
    using detail::AddWrapperMethods<AudioProgrammeReferenceScreen>::unset;

    void print(std::ostream &os) const {
      os << "(";
      if (has<CoordinateMode>()) {
        os << "coordinateMode=" << get<CoordinateMode>();
      }
      os << ")";
    }

   private:
    using detail::AudioProgrammeReferenceScreenBase::get;
    using detail::AudioProgrammeReferenceScreenBase::has;
    using detail::AudioProgrammeReferenceScreenBase::isDefault;
    using detail::AudioProgrammeReferenceScreenBase::unset;

    friend class detail::AddWrapperMethods<AudioProgrammeReferenceScreen>;
  };

}  // namespace adm
