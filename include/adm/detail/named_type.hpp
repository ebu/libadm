/// @file named_type.hpp
#pragma once

#include <iosfwd>
#include "adm/detail/named_type_validators.hpp"
#include "adm/detail/type_traits.hpp"

namespace adm {
  /// @brief Implementation details
  namespace detail {

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

      NamedType() : value_() { Validator::validate(get()); }
      explicit NamedType(T const& value) : value_(value) {
        Validator::validate(get());
      }
      explicit NamedType(T&& value) : value_(value) {
        Validator::validate(get());
      }
      T& get() { return value_; }
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
