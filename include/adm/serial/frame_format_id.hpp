/// @file frame_format_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_type_validators.hpp"

namespace adm {

  /// @brief Tag for FrameFormatIdValue
  struct FrameIndexTag {};
  /// @brief NamedType for the FrameFormatIdValue attribute
  using FrameIndex = detail::NamedType<unsigned int, FrameIndexTag/*,
                                       detail::FrameIndexValidator*/>; // TODO fix validators for default constructed NamedTypes

  struct ChunkIndexTag {};
  using ChunkIndex = detail::NamedType<unsigned int, ChunkIndexTag/*,
                                       detail::ChunkIndexValidator*/>; // TODO fix validators for default constructed NamedTypes

  /// @brief Tag for FrameFormatId
  struct FrameFormatIdTag {};
  namespace detail {
    using FrameFormatIdBase = HasParameters<RequiredParameter<FrameIndex>,
                                            OptionalParameter<ChunkIndex>>;
  }  // namespace detail

  /// @brief Representation of an FrameFormatId
  class FrameFormatId : private detail::FrameFormatIdBase,
                        private detail::AddWrapperMethods<FrameFormatId> {
   public:
    typedef FrameFormatIdTag tag;

    using detail::FrameFormatIdBase::get;
    using detail::FrameFormatIdBase::has;
    using detail::FrameFormatIdBase::isDefault;
    using detail::FrameFormatIdBase::set;
    using detail::FrameFormatIdBase::unset;
    using detail::AddWrapperMethods<FrameFormatId>::get;
    using detail::AddWrapperMethods<FrameFormatId>::has;
    using detail::AddWrapperMethods<FrameFormatId>::isDefault;
    using detail::AddWrapperMethods<FrameFormatId>::unset;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    explicit FrameFormatId(FrameIndex index, Parameters... optionalNamedArgs);
    ///@{
    /**
     * @brief Operator overload
     *
     * Compares the string representation of the FrameFormatId.
     */
    ADM_EXPORT bool operator==(const FrameFormatId& other) const;
    ADM_EXPORT bool operator!=(const FrameFormatId& other) const;
    ADM_EXPORT bool operator<(const FrameFormatId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

    friend class detail::AddWrapperMethods<FrameFormatId>;
  };

  // ---- Free functions ---- //

  /// @brief Parse an FrameFormatId string and convert it into an object
  ADM_EXPORT FrameFormatId parseFrameFormatId(const std::string& id);
  /// @brief Format an formatId object as string
  ADM_EXPORT std::string formatId(FrameFormatId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  FrameFormatId::FrameFormatId(FrameIndex frameIndex,
                               Parameters... optionalNamedArgs) {
    // TODO fix default constructed NamedType/Validators so this code can go
    auto frameIndexValue = frameIndex.get();
    if (frameIndexValue == 0 || frameIndexValue > 0xFFFFFFFF) {
      throw OutOfRangeError("FrameIndex " + std::to_string(frameIndexValue) +
                            " outside valid range of [0x1,0xFFFFFFFF]");
    }
    set(frameIndex);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
    // TODO fix default constructed NamedType/Validators so this code can go
    if (has<ChunkIndex>()) {
      auto chunkIndexValue = get<ChunkIndex>().get();
      if (chunkIndexValue == 0 || chunkIndexValue > 0xFF) {
        throw OutOfRangeError("ChunkIndex " + std::to_string(chunkIndexValue) +
                              " outside valid range of [0x1,0xFF]");
      }
    }
  };
}  // namespace adm
