#include "adm/write.hpp"
#include <fstream>
#include "adm/private/xml_writer.hpp"

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
    xml::XmlWriter writer(options);
    return writer.write(admDocument, stream);
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
    xml::XmlWriter writer(options);
    return writer.write(admFrame, stream);
  }
  
}  // namespace adm
