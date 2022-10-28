/// @file write.hpp
#pragma once
#include <string>
#include <memory>
#include <iosfwd>
#include "adm/detail/enum_bitmask.hpp"
#include "adm/export.h"

namespace adm {

  class Document;
  class Frame;

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
   * @brief Write an Frame
   *
   * Convenience wrapper for files using
   * `writeXml(std::ostream&, std::shared_ptr<const Frame>)`
   * @param filename XML file to write to
   * @param admFrame ADM frame that should be transformed into XML
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT void writeXml(
      const std::string& filename, std::shared_ptr<const Frame> admFrame,
      xml::WriterOptions options = xml::WriterOptions::none);

  /**
   * @brief Write an Frame to an output stream
   * @param stream output stream to write XML data
   * @param admFrame ADM frame that should be transformed into XML
   * @param options Options to influence the XML generator behaviour
   */
  ADM_EXPORT std::ostream& writeXml(
      std::ostream& stream, std::shared_ptr<const Frame> admFrame,
      xml::WriterOptions options = xml::WriterOptions::none);

  /**
   * @}
   */
}  // namespace adm

ENABLE_ENUM_BITMASK_OPERATORS(adm::xml::WriterOptions);
