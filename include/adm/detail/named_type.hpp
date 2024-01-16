/// @file named_type.hpp
#pragma once

#include <iosfwd>
#include "adm/detail/named_type_validators.hpp"
#include "adm/detail/type_traits.hpp"

namespace adm {
  /// @brief Implementation details
  namespace detail {

    /// Get the default value for some NamedType; specialise this to add custom
    /// defaults. Note that this is mainly used to make NamedType values
    /// default-constructable when the validator doesn't accept the default
    /// value of the underlying type.
    template <typename NT>
    NT getNamedTypeDefault() {
      return NT{typename NT::value_type{}};
    }

    /**
     * @brief Named type class
     *
     * For more background information about strong types have a look at:
     * https://www.fluentcpp.com/2016/12/05/named-constructors/
     */
    template <typename T, typename Tag,
              typename Validator = detail::DefaultValidator>
    class NamedType {
     public:
      typedef T value_type;
      typedef Tag tag;

      NamedType() : NamedType(getNamedTypeDefault<NamedType>()) {}

      explicit NamedType(T const& value) : value_(value) {
        Validator::validate(get());
      }
      explicit NamedType(T&& value) : value_(std::move(value)) {
        Validator::validate(get());
      }
      T const& get() const { return value_; }

      bool operator==(const NamedType<T, Tag, Validator>& other) const {
        return this->get() == other.get();
      }

      template <typename U>
      bool operator==(const U& other) const {
        return this->get() == other;
      }

      bool operator!=(const NamedType<T, Tag, Validator>& other) const {
        return this->get() != other.get();
      }

      template <typename U>
      bool operator!=(const U& other) const {
        return this->get() != other;
      }

      bool operator<(const NamedType<T, Tag, Validator>& other) const {
        return this->get() < other.get();
      }

      template <typename U>
      bool operator<(const U& other) const {
        return this->get() < other;
      }

      bool operator>(const NamedType<T, Tag, Validator>& other) const {
        return this->get() > other.get();
      }

      template <typename U>
      bool operator>(const U& other) const {
        return this->get() > other;
      }

      bool operator>=(const NamedType<T, Tag, Validator>& other) const {
        return this->get() >= other.get();
      }
      template <typename U>
      bool operator>=(const U& other) const {
        return this->get() >= other;
      }

      bool operator<=(const NamedType<T, Tag, Validator>& other) const {
        return this->get() <= other.get();
      }

      template <typename U>
      bool operator<=(const U& other) const {
        return this->get() <= other;
      }

      NamedType<T, Tag, Validator>& operator++() {
        ++value_;
        return *this;
      }

      NamedType<T, Tag, Validator> operator++(int) {
        NamedType<T, Tag, Validator> tmp = *this;
        operator++();
        return tmp;
      }

      NamedType<T, Tag, Validator>& operator--() {
        --value_;
        return *this;
      }

      NamedType<T, Tag, Validator> operator--(int) {
        NamedType<T, Tag, Validator> tmp = *this;
        operator--();
        return tmp;
      }

      T const* operator->() const { return &value_; }
      T const& operator*() const { return value_; }

     private:
      T value_;
    };

    template <typename T, typename Tag, typename Validator>
    std::ostream& operator<<(std::ostream& stream,
                             const NamedType<T, Tag, Validator>& rhs) {
      stream << rhs.get();
      return stream;
    }

  }  // namespace detail
}  // namespace adm
