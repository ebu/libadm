/// @file audio_block_format_hoa.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/elements/screen_ref.hpp"
#include "adm/elements/nfc_ref_dist.hpp"
#include "adm/elements/normalization.hpp"

namespace adm {

  class Document;

  /// @brief Tag for AudioBlockFormatHoa
  struct AudioBlockFormatHoaTag {};
  /// @brief Tag for NamedType ::Order
  struct OrderTag {};
  /// @brief NamedType for order parameter
  using Order = detail::NamedType<int, OrderTag>;
  /// @brief Tag for NamedType ::Degree
  struct DegreeTag {};
  /// @brief NamedType for degree parameter
  using Degree = detail::NamedType<int, DegreeTag>;
  /// @brief Tag for NamedType ::Equation
  struct EquationTag {};
  /// @brief NamedType for a equation parameter
  using Equation = detail::NamedType<std::string, EquationTag>;

  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "HOA"
   */
  class AudioBlockFormatHoa {
   public:
    typedef AudioBlockFormatHoaTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    AudioBlockFormatHoa(Order order,
                        Degree degree,
                        Parameters... optionalNamedArgs);

    ADM_EXPORT AudioBlockFormatHoa(const AudioBlockFormatHoa&) = default;
    ADM_EXPORT AudioBlockFormatHoa(AudioBlockFormatHoa&&) = default;
    ADM_EXPORT AudioBlockFormatHoa& operator=(const AudioBlockFormatHoa&) =
        default;
    ADM_EXPORT AudioBlockFormatHoa& operator=(AudioBlockFormatHoa&&) = default;
    /**
     * @brief Copy AudioBlockFormatHoa
     *
     * The actual copy constructor is private to ensure that an
     * AudioBlockFormatHoa can only be created as a `std::shared_ptr`.
     */
    ADM_EXPORT std::shared_ptr<AudioBlockFormatHoa> copy() const;

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

    /// @brief AudioBlockFormatId setter
    ADM_EXPORT void set(AudioBlockFormatId id);
    /// @brief Rtime setter
    ADM_EXPORT void set(Rtime rtime);
    /// @brief Duration setter
    ADM_EXPORT void set(Duration duration);
    /// @brief Order setter
    ADM_EXPORT void set(Order order);
    /// @brief Degree setter
    ADM_EXPORT void set(Degree degree);
    /// @brief NfcRefDist setter
    ADM_EXPORT void set(NfcRefDist dist);
    /// @brief ScreenRef setter
    ADM_EXPORT void set(ScreenRef screenRef);
    /// @brief Normalization setter
    ADM_EXPORT void set(Normalization normalization);
    /// @brief Equation setter
    ADM_EXPORT void set(Equation equation);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

   private:
    ADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT Order get(detail::ParameterTraits<Order>::tag) const;
    ADM_EXPORT Degree get(detail::ParameterTraits<Degree>::tag) const;
    ADM_EXPORT NfcRefDist get(detail::ParameterTraits<NfcRefDist>::tag) const;
    ADM_EXPORT ScreenRef get(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT Normalization get(detail::ParameterTraits<Normalization>::tag) const;
    ADM_EXPORT Equation get(detail::ParameterTraits<Equation>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Order>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Degree>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<NfcRefDist>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Normalization>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Equation>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<NfcRefDist>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<ScreenRef>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Normalization>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<NfcRefDist>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<ScreenRef>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Normalization>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Equation>::tag);
    AudioBlockFormatId id_;
    boost::optional<Rtime> rtime_;
    boost::optional<Duration> duration_;
    boost::optional<Order> order_;
    boost::optional<Degree> degree_;
    boost::optional<NfcRefDist> nfcRefDist_;
    boost::optional<ScreenRef> screenRef_;
    boost::optional<Normalization> normalization_;
    boost::optional<Equation> equation_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatHoa::AudioBlockFormatHoa(Order order,
      Degree degree, Parameters... optionalNamedArgs) {
    set(order);
    set(degree);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter AudioBlockFormatHoa::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatHoa::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatHoa::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioBlockFormatHoa::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
