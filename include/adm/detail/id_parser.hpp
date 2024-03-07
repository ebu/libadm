#pragma once

#include <algorithm>
#include <string>
#include <sstream>

namespace adm {
  namespace detail {
    inline bool id_starts_with(std::string const &id, const char *prefix,
                               std::size_t prefix_length) {
      return prefix_length <= id.size() &&
             std::equal(prefix, prefix + prefix_length, id.begin(),
                        id.begin() + prefix_length);
    }

    class IdParseException : public std::runtime_error {
     public:
      IdParseException(std::size_t index)
          : std::runtime_error("Id parser error - bad hex digit"),
            index{index} {}
      std::size_t index;
    };

    template <typename T>
    struct IdTraits;

    constexpr std::size_t strlen_constexpr(char const *start) {
      const char *end = start;
      while (*end != '\0') ++end;
      return end - start;
    }

    struct IdSection {};

    class IDParser {
     public:
      /// construct given non-owning reference to the ID string to parse
      explicit IDParser(std::string const &id) : id{id} {}

      /// check the length
      template <typename IdT>
      void check_size() {
        using TraitsT = IdTraits<IdT>;
        constexpr auto length{detail::strlen_constexpr(TraitsT::format)};
        if (id.size() != length) {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (wrong length, should be " << length
                      << " characters): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      /// check that the start of the ID matches the given prefix
      template <typename IdT>
      void check_prefix() const {
        using TraitsT = IdTraits<IdT>;
        constexpr auto prefix_length{strlen_constexpr(TraitsT::prefix)};
        if (!id_starts_with(id, TraitsT::prefix, prefix_length)) {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (incorrect prefix, should be '" << TraitsT::prefix
                      << "'): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      /// check that there's an underscore at the given position
      template <typename IdT>
      void check_underscore() {
        using TraitsT = IdTraits<IdT>;
        auto underscore_pos = TraitsT::underscore_position;
        assert(underscore_pos < id.size());
        if (id[underscore_pos] != '_') {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (expected underscore at char " << underscore_pos
                      << "): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      template <typename IdT>
      unsigned parse_hex(size_t start, size_t len) {
        try {
          return parse_hex_impl(start, len);
        } catch (IdParseException const &e) {
          using TraitsT = IdTraits<IdT>;
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (expected hex char at char" << e.index
                      << "): " << id;
          throw std::runtime_error(errorString.str());
        };
      }

      /// parse a hex value case-insensitively starting at start for len chars
      unsigned parse_hex_impl(size_t start, size_t len) {
        assert(start + len <= id.size());

        // parse manually -- all of the built-in methods would require copying
        // the string and/or lots of error checking to deal with things like
        // whitespace skipping and stopping at non-hex chars
        unsigned acc = 0;
        for (size_t i = start; i < start + len; i++) {
          char c = id[i];
          unsigned c_value;
          if ('0' <= c && c <= '9')
            c_value = c - '0';
          else if ('a' <= c && c <= 'f')
            c_value = c - ('a' - 10);
          else if ('A' <= c && c <= 'F')
            c_value = c - ('A' - 10);
          else {
            throw IdParseException(i);
          }
          acc = (acc << 4) | c_value;
        }

        return acc;
      }

     private:
      std::string const &id;
    };

    /// write a hex value into an existing string
    inline void formatHex(std::string &id, size_t start, size_t len,
                          unsigned value) {
      assert(start + len <= id.size());
      for (int i = static_cast<int>(start + len) - 1;
           i >= static_cast<int>(start); i--) {
        unsigned charValue = value & 0xf;
        id[i] = charValue < 10 ? '0' + charValue : ('a' - 10) + charValue;
        value >>= 4;
      }

      if (value) {
        std::ostringstream errorString;
        errorString << "failed to convert int value to hex: " << value;
        throw std::runtime_error(errorString.str());
      }
    }

  }  // namespace detail
}  // namespace adm
