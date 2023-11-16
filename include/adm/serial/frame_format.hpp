/// @file frame_format.hpp
#pragma once

#include <memory>
#include <chrono>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include "adm/elements/time.hpp"
#include "adm/serial/frame_format_id.hpp"
#include "adm/serial/changed_ids.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_type_validators.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/auto_base.hpp"
#include "adm/export.h"

namespace adm {

  class FrameHeader;

  /// @brief Tag for FlowId
  struct FlowIdTag {};
  /// @brief NamedType for the FlowId attribute
  using FlowId = detail::NamedType<std::string, FlowIdTag>;

  enum class TimeReferenceValue { TOTAL, LOCAL };
  /// @brief Tag for NamedType ::TimeReference
  struct TimeReferenceTag {};
  /// @brief NamedType for the TimeReference attribute
  using TimeReference = detail::NamedType<TimeReferenceValue, TimeReferenceTag>;

  std::string formatValue(TimeReferenceValue value);

  /// @brief Enum type for permitted values of FrameType
  enum class FrameTypeValue { HEADER, FULL, DIVIDED, INTERMEDIATE, ALL };
  /// @brief string conversion function for FrameTypeValue
  std::string formatValue(FrameTypeValue value);

  /// @brief Tag for NamedType ::FrameType
  struct FrameTypeTag {};
  /// @brief NamedType for the FrameType attribute
  using FrameType = detail::NamedType<FrameTypeValue, FrameTypeTag>;
  /// @brief Tag for NamedType ::CountToFull
  struct CountToFullTag {};
  /// @brief NamedType for the CountToFull attribute
  using CountToFull = detail::NamedType<unsigned int, CountToFullTag,
                                        detail::DefaultValidator>;
  /// @brief Tag for NamedType ::NumMetadataChunks
  struct NumMetadataChunksTag {};
  /// @brief NamedType for the NumMetadataChunks attribute
  using NumMetadataChunks =
      detail::NamedType<unsigned int, NumMetadataChunksTag,
                        detail::DefaultValidator>;
  /// @brief Tag for NamedType ::CountToSameChunk
  struct CountToSameChunkTag {};
  /// @brief NamedType for the CountToSameChunk attribute
  using CountToSameChunk = detail::NamedType<unsigned int, CountToSameChunkTag,
                                             detail::DefaultValidator>;

  /// @brief Tag for FrameFormat
  struct FrameFormatTag {};

  namespace detail {
    template <>
    TimeReference getDefault<TimeReference>();

    using FrameFormatBase =
        HasParameters<RequiredParameter<Start>, RequiredParameter<Duration>,
                      RequiredParameter<FrameType>,
                      DefaultParameter<TimeReference>,
                      OptionalParameter<NumMetadataChunks>,
        OptionalParameter<CountToSameChunk>, OptionalParameter<ChangedIds>,
        OptionalParameter<FlowId>>;
  }  // namespace detail

  /**
   * @ingroup sadm
   * @brief Class representation of the frameFormat ADM element
   *
   * \rst
   * +------------------------+--------------------------------+------------------------------+
   * | ADM Parameter          | Parameter Type                 | Pattern Type                 |
   * +========================+================================+==============================+
   * | frameFormatID          | :class:`FrameFormatId`         | :class:`RequiredParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | start                  | :type:`FrameStart`             | :class:`RequiredParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | duration               | :type:`FrameDuration`          | :class:`RequiredParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | type                   | :type:`FrameType`              | :class:`RequiredParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | timeReference          | :type:`TimeReference`          | :class:`DefaultParameter`    |
   * +------------------------+--------------------------------+------------------------------+
   * | flowId                 | :type:`FlowID`                 | :class:`OptionalParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | countToFull            | :type:`CountToFull`            | :class:`OptionalParameter`    |
   * +------------------------+--------------------------------+------------------------------+
   * | numMetadataChunks      | :type:`NumMetadataChunks`      | :class:`OptionalParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | countToSameChunk       | :type:`CountToSameChunk`       | :class:`OptionalParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * | changedIDs             | :type:`ChangedIds`             | :class:`OptionalParameter`   |
   * +------------------------+--------------------------------+------------------------------+
   * \endrst
   *
   * Note that:
   * - countToFull has defaults dependent on the flow type, as this is not known at this level they are not implemented
   * - numMetadataChunks is required for frame type divided and not permitted for other frame types.
   * - countToSameChunk is optional for frame type divided and not permitted for other frame types.
   */
  class FrameFormat : private detail::FrameFormatBase {
   public:
    using tag = FrameFormatTag;
    /// Type that holds the id for this element;
    using id_type = FrameFormatId;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    FrameFormat(FrameFormatId id, Start start, Duration duration,
                FrameType frameType, Parameters... optionalNamedArgs);
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

    using detail::FrameFormatBase::get;
    using detail::FrameFormatBase::has;
    using detail::FrameFormatBase::isDefault;
    using detail::FrameFormatBase::set;
    using detail::FrameFormatBase::unset;

    /// @brief FrameFormatId setter
    ADM_EXPORT void set(FrameFormatId id);
    /// @brief CountToFull setter
    ADM_EXPORT void set(CountToFull countToFull);

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
    ADM_EXPORT CountToFull get(detail::ParameterTraits<CountToFull>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<FrameFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FrameType>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<CountToFull>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<CountToFull>::tag);

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    FrameFormatId id_;
    boost::optional<CountToFull> countToFull_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  FrameFormat::FrameFormat(FrameFormatId id, Start start, Duration duration,
                           FrameType frameType, Parameters... optionalNamedArgs)
      : id_(std::move(id)) {
    detail::setNamedOptionHelper(this, std::move(start), std::move(duration),
                                 frameType, std::move(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter FrameFormat::get() const {
    using Tag = typename detail::ParameterTraits<Parameter>::tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool FrameFormat::has() const {
    using Tag = typename detail::ParameterTraits<Parameter>::tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool FrameFormat::isDefault() const {
    using Tag = typename detail::ParameterTraits<Parameter>::tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void FrameFormat::unset() {
    using Tag = typename detail::ParameterTraits<Parameter>::tag;
    return unset(Tag());
  }

}  // namespace adm
