/// @file write.hpp
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
     * of the XML writer.
     *
     * `WriterOptions` satisfies the requirements of
     * [BitmaskType](http://en.cppreference.com/w/cpp/concept/BitmaskType).
     *
     * This means that the bitwise operators (e.g. `operator|` or `operator&`)
     * are defined for this type. Thus options may be combined by `OR`-ing the
     * respective values.
     *
     * ### Available options
     * At most one writer option in each of the following options groups may
     * be present, otherwise the behavior is undefined.
     *
     * Constant             | Meaning
     * ---------------------| ----------------
     *                      || **options controlling the XML envelope**
     * none                 | use `<ebuCoreMain>` envelope (default)
     * itu_structure        | use `<ituADM>` to contain the ADM elements
     *                      || **options controlling default values**
     * none                 | use `<ebuCoreMain>` envelope (default)
     * write_default_values | use `<ebuCoreMain>` envelope (default)
     *
     *
     * @ingroup xml
     */
    enum class WriterOptions : unsigned {
      none = 0x0,  ///< default behaviour
      itu_structure = 0x1,  ///< use ITU xml structure
      write_default_values = 0x2,  ///< write default values
    };

    enum class SadmWriterOptions : unsigned {
      none = 0x0,  ///< default behaviour
      core_metadata = 0x1,  ///< audioFormatExtended inside coreMetadata/format/
      write_default_values = 0x2,  ///< write default values
    };
  }  // namespace xml

  /**
   * @addtogroup xml XML parsing and generation
   * @{
   */

  /**
   * @brief Write an Document
   *
   * Convenience wrapper for files using
   * `writeXml(std::ostream&, std::shared_ptr<const Document>)`
   * @param filename XML file to write to
   * @param admDocument ADM document that should be transformed into XML
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT void writeXml(
      const std::string& filename, std::shared_ptr<const Document> admDocument,
      xml::WriterOptions options = xml::WriterOptions::none);

  /**
   * @brief Write an Document to an output stream
   * @param stream output stream to write XML data
   * @param admDocument ADM document that should be transformed into XML
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT std::ostream& writeXml(
      std::ostream& stream, std::shared_ptr<const Document> admDocument,
      xml::WriterOptions options = xml::WriterOptions::none);

  /**
   * @brief Write a SADM Frame. The header will be written using the supplied FrameHeader
   * and the audioFormatExtended node will be written using the supplied Document.
   * Note that the header's TimeReference will be used to determine whether rtime/duration
   * or lstart/lduration is written in audioBlockFormats
   *
   * Convenience wrapper for files using
   * `writeXml(std::ostream&, std::shared_ptr<const Document>, FrameHeader const&)`
   * @param filename XML file to write to
   * @param admDocument ADM document to be used as frame's audioFormatExtended node
   * @param frameHeader SADM frame header
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT void writeXml(
      const std::string& filename, std::shared_ptr<const Document> admDocument,
      FrameHeader const& frameHeader,
      xml::SadmWriterOptions options = xml::SadmWriterOptions::none);

  /**
   * @brief Write a SADM Frame to an output stream. The header will be written using the supplied FrameHeader
   * and the audioFormatExtended node will be written using the supplied Document.
   * Note that the header's TimeReference will be used to determine whether rtime/duration
   * or lstart/lduration is written in audioBlockFormats
   * @param stream output stream to write XML data
   * @param admDocument ADM document to be used as frame's audioFormatExtended node
   * @param frameHeader SADM frame header
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT std::ostream& writeXml(
      std::ostream& stream, std::shared_ptr<const Document> admDocument,
      FrameHeader const& frameHeader,
      xml::SadmWriterOptions options = xml::SadmWriterOptions::none);
  /**
   * @}
   */
}  // namespace adm

ENABLE_ENUM_BITMASK_OPERATORS(adm::xml::WriterOptions);
ENABLE_ENUM_BITMASK_OPERATORS(adm::xml::SadmWriterOptions);
