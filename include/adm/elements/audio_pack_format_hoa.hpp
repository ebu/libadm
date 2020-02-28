/// @file audio_pack_format_hoa.hpp
#pragma once

#include "adm/elements/screen_ref.hpp"
#include "adm/elements/normalization.hpp"
#include "adm/elements/nfc_ref_dist.hpp"
#include "adm/elements/audio_pack_format.hpp"

namespace adm {

    class Document;

    class AudioPackFormatHoa : public AudioPackFormat {
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

      /// @brief ScreenRef setter
      ADM_EXPORT void set(ScreenRef screenRef);
      /// @brief Normalization setter
      ADM_EXPORT void set(Normalization normalization);
      /// @brief nfcRefDistance setter
      ADM_EXPORT void set(NfcRefDist nfcRefDist);

      /**
       * @brief ADM parameter unset template
       *
       * Templated unset method with the ADM parameter type as template
       * argument. Removes an ADM parameter if it is optional or resets it to
       * the default value if there is one.
       */
      template <typename Parameter>
      void unset();

      // ---- Inherited functions ---- //
      using AudioPackFormat::get;
      using AudioPackFormat::has;
      using AudioPackFormat::set;
      using AudioPackFormat::unset;

      /**
       * @brief Print overview to ostream
       */
      void print(std::ostream &os) const;

     private:
      friend class AudioPackFormatHoaAttorney;

      ADM_EXPORT AudioPackFormatHoa(AudioPackFormatName name);
      // ADM_EXPORT AudioPackFormatHoa(const AudioPackFormat &) = default;
      // ADM_EXPORT AudioPackFormatHoa(AudioPackFormat &&) = default;

      ADM_EXPORT ScreenRef get(detail::ParameterTraits<ScreenRef>::tag) const;
      ADM_EXPORT Normalization
          get(detail::ParameterTraits<Normalization>::tag) const;
      ADM_EXPORT NfcRefDist get(detail::ParameterTraits<NfcRefDist>::tag) const;

      ADM_EXPORT bool has(detail::ParameterTraits<ScreenRef>::tag) const;
      ADM_EXPORT bool has(detail::ParameterTraits<Normalization>::tag) const;
      ADM_EXPORT bool has(detail::ParameterTraits<NfcRefDist>::tag) const;

      ADM_EXPORT bool isDefault(detail::ParameterTraits<ScreenRef>::tag) const;
      ADM_EXPORT bool isDefault(
          detail::ParameterTraits<Normalization>::tag) const;
      ADM_EXPORT bool isDefault(detail::ParameterTraits<NfcRefDist>::tag) const;

      ADM_EXPORT void unset(detail::ParameterTraits<ScreenRef>::tag);
      ADM_EXPORT void unset(detail::ParameterTraits<Normalization>::tag);
      ADM_EXPORT void unset(detail::ParameterTraits<NfcRefDist>::tag);

      boost::optional<ScreenRef> screenRef_;
      boost::optional<Normalization> normalization_;
      boost::optional<NfcRefDist> nfcRefDist_;
    };

    // ---- Implementation ---- //
    template <typename... Parameters>
    std::shared_ptr<AudioPackFormatHoa> AudioPackFormatHoa::create(
        AudioPackFormatName name, Parameters... optionalNamedArgs) {
      std::shared_ptr<AudioPackFormatHoa> pack(new AudioPackFormatHoa(name));
      detail::setNamedOptionHelper(
          pack, std::forward<Parameters>(optionalNamedArgs)...);
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
