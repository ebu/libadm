#include "adm/xml_writer.hpp"
#include <fstream>
#include "adm/private/xml_composer.hpp"

namespace adm {

  void writeXml(const std::string& filename,
                std::shared_ptr<const Document> admDocument,
                xml::WriterOptions options) {
    std::ofstream stream(filename);
    writeXml(stream, admDocument, options);
  }

  std::ostream& writeXml(std::ostream& stream,
                         std::shared_ptr<const Document> admDocument,
                         xml::WriterOptions options) {
    xml::XmlComposer composer(options);

    composer.compose(admDocument);
    return composer.writeXmlTo(stream);
  }

  void writeXml(const std::string& filename,
                std::shared_ptr<const Frame> admFrame,
                xml::WriterOptions options) {
    std::ofstream stream(filename);
    writeXml(stream, admFrame, options);
  }

  std::ostream& writeXml(std::ostream& stream,
                         std::shared_ptr<const Frame> admFrame,
                         xml::WriterOptions options) {
    xml::XmlComposer composer(options);

    composer.compose(admFrame);
    return composer.writeXmlTo(stream);
  }

}  // namespace adm
