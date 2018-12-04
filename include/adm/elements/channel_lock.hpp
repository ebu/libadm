/// @file channel_lock.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/detail/named_type.hpp"

#include "adm/detail/named_option_helper.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::ChannelLockFlag
  struct ChannelLockFlagTag {};
  /// @brief NamedType for channelLockFlag parameter
  using ChannelLockFlag = detail::NamedType<bool, ChannelLockFlagTag>;
  /// @brief Tag for NamedType ::MaxDistance
  struct MaxDistanceTag {};
  /**
   * @brief NamedType for the maxDistance attribute
   *
   * Valid values are in the range [0, 2].
   */
  using MaxDistance =
      detail::NamedType<float, MaxDistanceTag, detail::RangeValidator<0, 2>>;

  /// @brief Tag for ChannelLock class
  struct ChannelLockTag {};
  /**
   * @brief ADM parameter class to specify a channel lock
   */
  class ChannelLock {
   public:
    typedef ChannelLockTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    ChannelLock(Parameters... optionalNamedArgs);

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

    /// @brief ChannelLockFlag setter
    ADM_EXPORT void set(ChannelLockFlag channelLockFlag);
    /// @brief MaxDistance setter
    ADM_EXPORT void set(MaxDistance maxDistance);

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
    ADM_EXPORT ChannelLockFlag
        get(detail::ParameterTraits<ChannelLockFlag>::tag) const;
    ADM_EXPORT MaxDistance get(detail::ParameterTraits<MaxDistance>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<ChannelLockFlag>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<MaxDistance>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<ChannelLockFlag>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<ChannelLockFlag>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<MaxDistance>::tag);

    boost::optional<ChannelLockFlag> channelLockFlag_;
    boost::optional<MaxDistance> maxDistance_;

    static const ChannelLockFlag channelLockFlagDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Helper function to check if the ::ChannelLockFlag of a
  /// ChannelLock is true
  ADM_EXPORT bool isEnabled(ChannelLock &channelLock);
  /// @brief Helper function to set the ::ChannelLockFlag of a ChannelLock
  /// to true
  ADM_EXPORT void enable(ChannelLock &channelLock);
  /// @brief Helper function to set the ::ChannelLockFlag of a ChannelLock
  /// to false
  ADM_EXPORT void disable(ChannelLock &channelLock);

  // ---- Implementation ---- //

  template <typename... Parameters>
  ChannelLock::ChannelLock(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter ChannelLock::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool ChannelLock::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool ChannelLock::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void ChannelLock::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
