/// @file audio_block_format_hoa.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/common_parameters.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"

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

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
      OptionalParameter<Order>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
      OptionalParameter<Degree>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
      OptionalParameter<Equation>;

    using AudioBlockFormatHoaBase =
        HasParameters<RequiredParameter<AudioBlockFormatId>,
                      DefaultParameter<Rtime>, OptionalParameter<Duration>,
                      OptionalParameter<Lstart>, OptionalParameter<Lduration>, 
                      OptionalParameter<InitializeBlock>, 
                      OptionalParameter<Order>, OptionalParameter<Degree>,
                      DefaultParameter<Normalization>,
                      DefaultParameter<NfcRefDist>,
                      DefaultParameter<Gain>,
                      DefaultParameter<Importance>,
                      DefaultParameter<HeadLocked>,
                      DefaultParameter<HeadphoneVirtualise>,
                      DefaultParameter<ScreenRef>,
                      OptionalParameter<Equation>>;
  }  // namespace detail

  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "HOA"
   *
   * Supported parameters are as follows:
   *
   * \rst
   * +---------------------+------------------------------+----------------------------+
   * | ADM Parameter       | Parameter Type               | Pattern Type               |
   * +=====================+==============================+============================+
   * | audioBlockFormatId  | :class:`AudioBlockFormatId`  | :class:`RequiredParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | rtime               | :type:`Rtime`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | duration            | :type:`Duration`             | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lstart              | :type:`Lstart`               | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lduration           | :type:`Lduration`            | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | initializeBlock     | :type:`InitializeBlock`      | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | order               | :type:`Order`                | :class:`RequiredParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | degree              | :type:`Degree`               | :class:`RequiredParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | normalization       | :type:`Normalization`        | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | nfcRefDist          | :type:`NfcRefDist`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | gain                | :class:`Gain`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | importance          | :type:`Importance`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | headLocked          | :type:`HeadLocked`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | headphoneVirtualise | :class:`HeadphoneVirtualise` | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | screenRef           | :type:`ScreenRef`            | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | equation            | :type:`Equation`             | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * \endrst
   */
  class AudioBlockFormatHoa : private detail::AudioBlockFormatHoaBase {
   public:
    typedef AudioBlockFormatHoaTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    AudioBlockFormatHoa(Order order, Degree degree,
                        Parameters... optionalNamedArgs);

    ADM_EXPORT AudioBlockFormatHoa(const AudioBlockFormatHoa&) = default;
    ADM_EXPORT AudioBlockFormatHoa(AudioBlockFormatHoa&&) = default;
    ADM_EXPORT AudioBlockFormatHoa& operator=(const AudioBlockFormatHoa&) =
        default;
    ADM_EXPORT AudioBlockFormatHoa& operator=(AudioBlockFormatHoa&&) = default;

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

    using detail::AudioBlockFormatHoaBase::set;

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
    using detail::AudioBlockFormatHoaBase::get;
    using detail::AudioBlockFormatHoaBase::has;
    using detail::AudioBlockFormatHoaBase::isDefault;
    using detail::AudioBlockFormatHoaBase::unset;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatHoa::AudioBlockFormatHoa(Order order, Degree degree,
                                           Parameters... optionalNamedArgs) {
    set(order);
    set(degree);
    detail::setNamedOptionHelper(
        this, std::move(optionalNamedArgs)...);
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
