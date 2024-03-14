#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <utility>

namespace adm {
  namespace detail {
    class IdParseException : public std::runtime_error {
     public:
      IdParseException(std::size_t index)
          : std::runtime_error("Id parser error - bad hex digit"),
            index{index} {}
      std::size_t index;
    };

    template <typename T>
    struct IdTraits;

    template <typename IdT, std::size_t index>
    struct IdSection;

    template <typename T>
    struct IdTypeFor {
      using type = T;
    };

    // don't want to pollute adm::detail with a bunch of functions
    // but also don't want to put them in an implementation file
    namespace id {

      constexpr std::size_t strlen_constexpr(char const *start) {
        const char *end = start;
        while (*end != '\0') ++end;
        return end - start;
      }

      constexpr std::size_t find_first(char const *format, char identifier) {
        std::size_t i = 0;
        while (i != strlen_constexpr(format)) {
          if (format[i] == identifier) break;
          ++i;
        }
        return i;
      }
      constexpr std::size_t find_last(char const *format, char identifier) {
        std::size_t i = strlen_constexpr(format);
        while (i != 0) {
          if (format[--i] == identifier) break;
        }
        return i;
      }
      constexpr std::size_t prefix_length(char const *format) {
        return find_first(format, '_') + 1u;
      }

      constexpr std::size_t underscore_position(char const *format) {
        return find_first(format + prefix_length(format), '_') +
               prefix_length(format);
      }

      constexpr bool has_underscore(char const *format) {
        return underscore_position(format) != strlen_constexpr(format);
      }

      inline bool id_starts_with(std::string const &id, const char *format,
                                 std::size_t prefix_length) {
        return prefix_length <= id.size() &&
               std::equal(format, format + prefix_length, id.begin(),
                          id.begin() + prefix_length);
      }

      ////////// Parsing

      /// parse a hex value case-insensitively starting at start for len chars
      inline unsigned parse_hex(std::string const &id, size_t start,
                                size_t len) {
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
    }  // namespace id

    template <typename IdT>
    class IDParser {
     public:
      /// construct given non-owning reference to the ID string to parse
      explicit IDParser(std::string const &id) : id{id} {}

      void validate() {
        check_prefix();
        check_size();
        check_underscore();
      }

      auto parse() {
        constexpr auto sectionCount = TraitsT::sections;
        return parseSections(std::make_index_sequence<sectionCount>());
      }

     private:
      using TraitsT = IdTraits<IdT>;
      template <std::size_t index>
      using SectionT = IdSection<IdT, index>;

      /// check that the start of the ID matches the given prefix
      void check_prefix() const {
        constexpr auto prefix_l{id::prefix_length(TraitsT::format)};
        if (!id::id_starts_with(id, TraitsT::format, prefix_l)) {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (incorrect prefix, should be '"
                      << std::string(TraitsT::format, prefix_l) << "'): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      // check that the id string is the correct length
      void check_size() {
        constexpr auto length{id::strlen_constexpr(TraitsT::format)};
        if (id.size() != length) {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (wrong length, should be " << length
                      << " characters): " << id;
          throw std::runtime_error(errorString.str());
        }
      }

      /// check that there's an underscore at the correct position
      void check_underscore() {
        auto constexpr underscore_pos =
            id::underscore_position(TraitsT::format);
        auto constexpr underscore = id::has_underscore(TraitsT::format);
        if (underscore) {
          assert(underscore_pos < id.size());
          if (id[underscore_pos] != '_') {
            std::ostringstream errorString;
            errorString << "invalid " << TraitsT::name
                        << " (expected underscore at char " << underscore_pos
                        << "): " << id;
            throw std::runtime_error(errorString.str());
          }
        }
      }

      unsigned parse_hex(size_t start, size_t len) {
        try {
          return id::parse_hex(id, start, len);
        } catch (IdParseException const &e) {
          std::ostringstream errorString;
          errorString << "invalid " << TraitsT::name
                      << " (expected hex char at char" << e.index
                      << "): " << id;
          throw std::runtime_error(errorString.str());
        };
      }

      template <std::size_t index>
      auto parseSection() {
        using Section = SectionT<index>;
        using ReturnT = typename Section::type;
        constexpr auto first{
            id::find_first(TraitsT::format, Section::identifier)};
        constexpr auto last{
            id::find_last(TraitsT::format, Section::identifier) + 1u};
        constexpr auto size = last - first;
        return ReturnT(this->parse_hex(first, size));
      }

      template <std::size_t... indexes>
      auto parseSections(std::index_sequence<indexes...>) {
        using ReturnT = typename IdTypeFor<IdT>::type;
        return ReturnT{parseSection<indexes>()...};
      }

      std::string const &id;
    };

    ////////// Formatting

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

    namespace id {
      template <typename TagT, std::size_t index, typename IdT>
      void formatSection(IdT const &id, std::string &output) {
        using TraitsT = IdTraits<TagT>;
        using SectionT = IdSection<TagT, index>;
        constexpr auto first{find_first(TraitsT::format, SectionT::identifier)};
        constexpr auto last{find_last(TraitsT::format, SectionT::identifier) +
                            1u};
        constexpr auto size = last - first;
        formatHex(output, first, size,
                  id.template get<typename SectionT::type>().get());
      }

      template <typename TagT, typename IdT>
      void formatSections(IdT const &, std::string const &,
                          std::index_sequence<>) {}

      template <typename TagT, typename IdT, std::size_t HeadIndex,
                std::size_t... Tail>
      void formatSections(IdT const &id, std::string &output,
                          std::index_sequence<HeadIndex, Tail...>) {
        formatSections<TagT>(id, output, std::index_sequence<Tail...>());
        formatSection<TagT, HeadIndex>(id, output);
      }
    }  // namespace id

    template <typename TagT, typename IdT>
    std::string formatId(IdT const &id) {
      using TraitsT = IdTraits<TagT>;
      std::string s{TraitsT::format};
      id::formatSections<TagT>(id, s,
                               std::make_index_sequence<TraitsT::sections>());
      return s;
    }

    template <typename IdT>
    std::string formatId(IdT const &id) {
      return formatId<IdT, IdT>(id);
    }

  }  // namespace detail
}  // namespace adm
