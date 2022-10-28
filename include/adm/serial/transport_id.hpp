/// @file transport_track_format_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for TransportIdValue
  struct TransportIdValueTag {};
  /// @brief NamedType for the TransportIdValue attribute
  using TransportIdValue = detail::NamedType<unsigned int, TransportIdValueTag>;

  /// @brief Tag for TransportId
  struct TransportIdTag {};
  /// @brief Representation of an TransportIdTag
  class TransportId {
   public:
    typedef TransportId tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    TransportId(Parameters... optionalNamedArgs);

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

    /// @brief Set value
    ADM_EXPORT void set(TransportIdValue value);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    ///@{
    /**
     * @brief Operator overload
     *
     * Compares the string representation of the TransportId.
     */
    ADM_EXPORT bool operator==(const TransportId& other) const;
    ADM_EXPORT bool operator!=(const TransportId& other) const;
    ADM_EXPORT bool operator<(const TransportId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT TransportIdValue
        get(detail::ParameterTraits<TransportIdValue>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<TransportIdValue>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<TransportIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TransportIdValue>::tag);

    boost::optional<TransportIdValue> value_;

    static const TransportIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an TransportId string and convert it into an
  /// object
  ADM_EXPORT TransportId parseTransportId(const std::string& id);
  /// @brief Format an TransportId object as string
  ADM_EXPORT std::string formatId(TransportId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  TransportId::TransportId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter TransportId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool TransportId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool TransportId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void TransportId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
