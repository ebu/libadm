/// @file screen_edge_lock.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedTyoe ::ScreenEdge
  struct ScreenEdgeTag {};
  /**
   * @brief NamedType for the screen edge
   *
   * Valid values are "left", "right", "top" and "bottom"
   */
  using ScreenEdge = detail::NamedType<std::string, ScreenEdgeTag,
                                       detail::ScreenEdgeValidator>;
  /// @brief Tag for NamedType ::HorizontalEdge
  struct HorizontalEdgeTag {};
  /**
   * @brief NamedType for the horizontal screen edge
   *
   * Valid values are "left" and "right"
   */
  using HorizontalEdge = detail::NamedType<std::string, HorizontalEdgeTag,
                                           detail::HorizontalEdgeValidator>;
  /// @brief Tag for NamedType ::VerticalEdge
  struct VerticalEdgeTag {};
  /**
   * @brief NamedType for the vertical screen edge
   *
   * Valid values are "top" and "bottom"
   */
  using VerticalEdge = detail::NamedType<std::string, VerticalEdgeTag,
                                         detail::VerticalEdgeValidator>;

  /// @brief Tag for ScreenEdgeLock class
  struct ScreenEdgeLockTag {};
  /**
   * @brief ADM parameter class to specify a screen edge lock
   */
  class ScreenEdgeLock {
   public:
    typedef ScreenEdgeLockTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    ScreenEdgeLock(Parameters... optionalNamedArgs);

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

    /// @brief ScreenEdge setter
    ADM_EXPORT void set(ScreenEdge screenEdge);
    /// @brief HorizontalEdge setter
    ADM_EXPORT void set(HorizontalEdge horizontalEdge);
    /// @brief VerticalEdge setter
    ADM_EXPORT void set(VerticalEdge verticalEdge);

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
    ADM_EXPORT HorizontalEdge
        get(detail::ParameterTraits<HorizontalEdge>::tag) const;
    ADM_EXPORT VerticalEdge
        get(detail::ParameterTraits<VerticalEdge>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<HorizontalEdge>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<VerticalEdge>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<HorizontalEdge>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<VerticalEdge>::tag);

    boost::optional<HorizontalEdge> horizontal_;
    boost::optional<VerticalEdge> vertical_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  ScreenEdgeLock::ScreenEdgeLock(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter ScreenEdgeLock::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool ScreenEdgeLock::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool ScreenEdgeLock::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void ScreenEdgeLock::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
