/// @file parse.hpp
#pragma once
#include <string>
#include <memory>
#include <iosfwd>
#include "adm/detail/enum_bitmask.hpp"
#include "adm/export.h"

namespace adm {

  class Document;
  class FrameHeader;

  namespace xml {
    /**
     * @brief Representation of available options to influence the behaviour
     * of the XML parser.
     *
     * `ParserOptions` satisfies the requirements of
     * [BitmaskType](http://en.cppreference.com/w/cpp/concept/BitmaskType).
     *
     * This means that the bitwise operators (e.g. `operator|` or `operator&`)
     * are defined for this type. Thus options may be combined by `OR`-ing the
     * respective values.
     *
     * @ingroup xml
     */
    enum class ParserOptions : unsigned {
      none = 0x0,  ///< default behaviour
      recursive_node_search =
          0x1,  ///< recursively search whole xml for audioFormatExtended node
      permit_time_reference_mismatch =
          0x2  ///< do not report a mismatch between the FrameHeader TimeReference and audioBlockFormat lstart/rtime lduration/duration as an error
    };
  }  // namespace xml

  /**
   * @addtogroup xml XML parsing and generation
   * @brief XML parsing and generation
   * @{
   */

  /**
   * @brief Parse an XML representation of the Audio Definition Model
   *
   * Convenience wrapper for files using `parseXml(std::istream&)`
   * @param filename XML file to read and parse
   * @param options Options to influence the XML parser behaviour
   */
  ADM_EXPORT std::shared_ptr<Document> parseXml(
      const std::string& filename,
      xml::ParserOptions options = xml::ParserOptions::none);

  /**
   * @brief Parse an xml document containing an audioFormatExtended
   * node into an adm::Document, using a SADM FrameHeader to check for consistency.
   * Primarily intended for parsing sadm frames.
   *
   * Convenience wrapper for files using `parseXml(std::istream&)`
   * @param filename XML file to read and parse
   * @param options Options to influence the XML parser behaviour
   */
  ADM_EXPORT std::shared_ptr<Document> parseXml(
      const std::string& filename, FrameHeader const& header,
      xml::ParserOptions options = xml::ParserOptions::none);

  /**
   * @brief Parse an XML representation of the Audio Definition Model
   *
   * Parse adm data from an `std::istream`.
   * @param stream input stream to parse XML data
   * @param options Options to influence the XML parser behaviour
   */
  ADM_EXPORT std::shared_ptr<Document> parseXml(
      std::istream& stream,
      xml::ParserOptions options = xml::ParserOptions::none);

  /**
   * @brief Parse an xml document containing an audioFormatExtended
   * node into an adm::Document, using a SADM FrameHeader to check for consistency.
   * Primarily intended for parsing sadm frames.
   *
   * Parse adm data from an `std::istream`.
   * @param stream input stream to parse XML data
   * @param options Options to influence the XML parser behaviour
   */
  ADM_EXPORT std::shared_ptr<Document> parseXml(
      std::istream& stream, FrameHeader const& header,
      xml::ParserOptions options = xml::ParserOptions::none);

  /**
   * @brief Parse an XML representation of a serial ADM frame and return
   * the frameHeader element as an adm::FrameHeader object
   *
   * Parse data from a `std::istream`.
   * @param stream input stream from which to parse XML data
   * @param options Options to influence the parser behaviour
   */
  ADM_EXPORT FrameHeader
  parseFrameHeader(std::istream& stream,
                   adm::xml::ParserOptions = adm::xml::ParserOptions::none);

  /**
   * @brief Parse an XML representation of a serial ADM frame and return
   * the frameHeader element as an adm::FrameHeader object
   *
   * Parse data from a file.
   * @param fileName Name of the input file from which to parse XML data
   * @param options Options to influence the parser behaviour
   */
  ADM_EXPORT FrameHeader
  parseFrameHeader(std::string const& fileName,
                   adm::xml::ParserOptions = adm::xml::ParserOptions::none);

  /**
   * @}
   */
}  // namespace adm

ENABLE_ENUM_BITMASK_OPERATORS(adm::xml::ParserOptions);
