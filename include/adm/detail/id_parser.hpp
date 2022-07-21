#pragma once

#include <string>
#include <sstream>

namespace adm {
  namespace detail {
    /// object used to help with ID parsing
    ///
    /// construct one with the tyep name (for errors) and the ID string to
    /// parse, then call the methods to check and parse different aspects of
    /// the ID
    class IDParser {
     public:
      /// construct given the type name for errors, and the ID string to parse
      ///
      /// this stores type_ and id_ but owns neither -- they must be kept alive
      /// externally
      IDParser(const char *type_, const std::string &id_)
          : type(type_), id(id_) {}

      /// check the length
      void check_size(size_t size) {
        if (id.size() != size) {
          std::ostringstream errorString;
          errorString << "invalid " << type << " (wrong length): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      /// check that the start of the ID matches the given prefix
      void check_prefix(const char *prefix, size_t prefix_len) {
        assert(prefix[prefix_len] == 0);
        for (size_t i = 0; i < prefix_len; i++)
          if (id[i] != prefix[i]) {
            std::ostringstream errorString;
            errorString << "invalid " << type << " (wrong prefix): " << id;
            throw std::runtime_error(errorString.str());
          }
      }

      /// check that there's an inderscore at the given position
      void check_underscore(size_t pos) {
        assert(pos < id.size());
        if (id[pos] != '_') {
          std::ostringstream errorString;
          errorString << "invalid " << type << " (expected underscore at char "
                      << pos << "): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      /// parse a hex value case-insensitively starting at start for len chars
      unsigned parse_hex(size_t start, size_t len) {
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
            std::ostringstream errorString;
            errorString << "invalid " << type << " (expected hex char at char"
                        << i << "): " << id;
            throw std::runtime_error(errorString.str());
          }
          acc = (acc << 4) | c_value;
        }

        return acc;
      }

     private:
      const char *type;
      const std::string &id;
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
