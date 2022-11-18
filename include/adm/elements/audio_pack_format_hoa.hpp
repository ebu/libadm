/// @file audio_pack_format_hoa.hpp
#pragma once

#include "adm/elements/common_parameters.hpp"
#include "adm/elements/screen_ref.hpp"
#include "adm/elements/normalization.hpp"
#include "adm/elements/nfc_ref_dist.hpp"
#include "adm/elements/audio_pack_format.hpp"

namespace adm {

    class Document;

    namespace detail {
      using AudioPackFormatHoaBase =
          HasParameters<DefaultParameter<Normalization>,
                        DefaultParameter<NfcRefDist>,
                        DefaultParameter<ScreenRef>>;
    }

    /**
     * @brief Class representation of the audioPackFormat ADM element for HOA
     *
     * \rst
     * Supported parameters are those from :class:`AudioPackFormat`, as well as:
     *
     * +---------------+-----------------------+---------------------------+
     * | ADM Parameter | Parameter Type        | Pattern Type              |
     * +===============+=======================+===========================+
     * | normalization | :type:`Normalization` | :class:`DefaultParameter` |
     * +---------------+-----------------------+---------------------------+
     * | nfcRefDist    | :type:`NfcRefDist`    | :class:`DefaultParameter` |
     * +---------------+-----------------------+---------------------------+
     * | screenRef     | :type:`ScreenRef`     | :class:`DefaultParameter` |
     * +---------------+-----------------------+---------------------------+
     * \endrst
     */
    class AudioPackFormatHoa : public AudioPackFormat,
                               private detail::AudioPackFormatHoaBase {
     public:
      template <typename... Parameters>
      static std::shared_ptr<AudioPackFormatHoa> create(
          AudioPackFormatName name, Parameters... optionalNamedArgs);

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

      using detail::AudioPackFormatHoaBase::set;

      /**
       * @brief ADM parameter unset template
       *
       * Templated unset method with the ADM parameter type as template
       * argument. Removes an ADM parameter if it is optional or resets it to
       * the default value if there is one.
       */
      template <typename Parameter>
      void unset();

      using AudioPackFormat::set;

      /**
       * @brief Print overview to ostream
       */
      ADM_EXPORT void print(std::ostream &os) const;

     private:
      using AudioPackFormat::get;
      using AudioPackFormat::has;
      using AudioPackFormat::unset;
      using AudioPackFormat::isDefault;

      using detail::AudioPackFormatHoaBase::get;
      using detail::AudioPackFormatHoaBase::has;
      using detail::AudioPackFormatHoaBase::unset;
      using detail::AudioPackFormatHoaBase::isDefault;

      friend class AudioPackFormatHoaAttorney;

      ADM_EXPORT explicit AudioPackFormatHoa(AudioPackFormatName name);
      // ADM_EXPORT AudioPackFormatHoa(const AudioPackFormat &) = default;
      // ADM_EXPORT AudioPackFormatHoa(AudioPackFormat &&) = default;
    };

    // ---- Implementation ---- //
    template <typename... Parameters>
    std::shared_ptr<AudioPackFormatHoa> AudioPackFormatHoa::create(
        AudioPackFormatName name, Parameters... optionalNamedArgs) {
      std::shared_ptr<AudioPackFormatHoa> pack(new AudioPackFormatHoa(name));
      detail::setNamedOptionHelper(
          pack, std::move(optionalNamedArgs)...);
      return pack;
    }

    template <typename Parameter>
    Parameter AudioPackFormatHoa::get() const {
      typedef typename detail::ParameterTraits<Parameter>::tag Tag;
      return get(Tag());
    }

    template <typename Parameter>
    bool AudioPackFormatHoa::has() const {
      typedef typename detail::ParameterTraits<Parameter>::tag Tag;
      return has(Tag());
    }

    template <typename Parameter>
    bool AudioPackFormatHoa::isDefault() const {
      typedef typename detail::ParameterTraits<Parameter>::tag Tag;
      return isDefault(Tag());
    }

    template <typename Parameter>
    void AudioPackFormatHoa::unset() {
      typedef typename detail::ParameterTraits<Parameter>::tag Tag;
      return unset(Tag());
    }

}  // namespace adm
