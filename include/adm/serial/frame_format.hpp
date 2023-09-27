/// @file frame_format.hpp
#pragma once

#include <memory>
#include <chrono>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include "adm/elements/time.hpp"
#include "adm/serial/frame_format_id.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_type_validators.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/export.h"

namespace adm {

  class FrameHeader;

  /// @brief Tag for FlowId
  struct FlowIdTag {};
  /// @brief NamedType for the FlowId attribute
  using FlowId = detail::NamedType<std::string, FlowIdTag>;
  
  /// @brief Tag for NamedType ::TimeReference
  struct TimeReferenceTag {};
  /// @brief NamedType for the TimeReference attribute
  using TimeReference = detail::NamedType<std::string, TimeReferenceTag,
                                          detail::TimeReferenceValueValidator>;
  /// @brief Tag for NamedType ::FrameType
  struct FrameTypeTag {};
  /// @brief NamedType for the FrameType attribute
  using FrameType = detail::NamedType<std::string, FrameTypeTag,
                                      detail::FrameTypeValueValidator>;
  /// @brief Tag for NamedType ::CountToFull
  struct CountToFullTag {};
  /// @brief NamedType for the CountToFull attribute
  using CountToFull = detail::NamedType<unsigned int, CountToFullTag,
                                        detail::DefaultValidator>;
  /// @brief Tag for NamedType ::NumSubFrame
  struct NumSubFrameTag {};
  /// @brief NamedType for the NumSubFrame attribute
  using NumSubFrame = detail::NamedType<unsigned int, NumSubFrameTag>;
  /// @brief Tag for NamedType ::FrameSkip
  struct FrameSkipTag {};
  /// @brief NamedType for the FrameSkip attribute
  using FrameSkip = detail::NamedType<unsigned int, FrameSkipTag>;
  struct FrameShiftTag {};
  /// @brief NamedType for the FrameShift attribute
  using FrameShift = detail::NamedType<unsigned int, FrameShiftTag>;

  /// @brief Tag for FrameFormat
  struct FrameFormatTag {};
  /**
   * @ingroup sadm
   * @brief Class representation of the frameFormat SADM element
   *
   * Supported parameters:
   *   - FrameFormatId
   *   - ::FrameStart
   *   - ::FrameDuration
   *   - ::TimeReference
   *   - ::FrameType
   *   - ::CountToFull
   *   - ::NumSubFrame
   *   - ::FrameSkip
   *   - ::FrameShift
   */

  class FrameFormat {
   public:
    typedef FrameFormatTag tag;
    /// Type that holds the id for this element;
    typedef FrameFormatId id_type;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    FrameFormat(FrameStart start, FrameDuration duration, FrameType frameType,
                Parameters... optionalNamedArgs);
    ADM_EXPORT FrameFormat(const FrameFormat &) = default;
    ADM_EXPORT FrameFormat(FrameFormat &&) = default;
    ADM_EXPORT FrameFormat &operator=(const FrameFormat &) = default;
    ADM_EXPORT FrameFormat &operator=(FrameFormat &&) = default;

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

    /// @brief FrameFormatId setter
    ADM_EXPORT void set(FrameFormatId id);
    /// @brief FrameStart setter
    ADM_EXPORT void set(FrameStart start);
    /// @brief FrameDuration setter
    ADM_EXPORT void set(FrameDuration duration);
    /// @brief TimeReference setter
    ADM_EXPORT void set(TimeReference timeReference);
    /// @brief FlowId setter
    ADM_EXPORT void set(FlowId id);
    /// @brief FrameType setter
    ADM_EXPORT void set(FrameType frameType);
    /// @brief CountToFull setter
    ADM_EXPORT void set(CountToFull countToFull);
    /// @brief numSubFrame setter
    ADM_EXPORT void set(NumSubFrame numSubFrame);
    /// @brief FrameSkip setter
    ADM_EXPORT void set(FrameSkip frameSkip);
    /// @brief FrameShift setter
    ADM_EXPORT void set(FrameShift frameShift);

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
    ADM_EXPORT FrameFormatId
        get(detail::ParameterTraits<FrameFormatId>::tag) const;
    ADM_EXPORT FrameStart get(detail::ParameterTraits<FrameStart>::tag) const;
    ADM_EXPORT FrameDuration
        get(detail::ParameterTraits<FrameDuration>::tag) const;
    ADM_EXPORT TimeReference
        get(detail::ParameterTraits<TimeReference>::tag) const;
    ADM_EXPORT FlowId get(detail::ParameterTraits<FlowId>::tag) const;
    ADM_EXPORT FrameType get(detail::ParameterTraits<FrameType>::tag) const;
    ADM_EXPORT CountToFull get(detail::ParameterTraits<CountToFull>::tag) const;
    ADM_EXPORT NumSubFrame get(detail::ParameterTraits<NumSubFrame>::tag) const;
    ADM_EXPORT FrameSkip get(detail::ParameterTraits<FrameSkip>::tag) const;
    ADM_EXPORT FrameShift get(detail::ParameterTraits<FrameShift>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<FrameFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameStart>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameDuration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<TimeReference>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FlowId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameType>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<CountToFull>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<NumSubFrame>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameSkip>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameShift>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<TimeReference>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<FlowId>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<CountToFull>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<NumSubFrame>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<FrameSkip>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<FrameShift>::tag);

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void isDefault(detail::ParameterTraits<TimeReference>::tag);

    FrameFormatId id_;
    FrameStart start_;
    FrameDuration duration_;
    FrameType frameType_;
    boost::optional<TimeReference> timeReference_;
    boost::optional<FlowId> flowId_;
    boost::optional<CountToFull> countToFull_;
    boost::optional<NumSubFrame> numSubFrame_;
    boost::optional<FrameSkip> frameSkip_;
    boost::optional<FrameShift> frameShift_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  FrameFormat::FrameFormat(FrameStart start, FrameDuration duration,
                           FrameType frameType, Parameters... optionalNamedArgs)
      : start_(start), duration_(duration), frameType_(frameType) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter FrameFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool FrameFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool FrameFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void FrameFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
