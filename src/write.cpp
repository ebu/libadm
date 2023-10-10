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
                    std::shared_ptr<const Document> admDocument,
                    FrameHeader const& frameHeader,
                    xml::SadmWriterOptions options) {
    std::ofstream stream(filename);
    writeXml(stream, admDocument, frameHeader, options);
  }

  std::ostream& writeXml(std::ostream& stream,
                             std::shared_ptr<const Document> admDocument,
                             FrameHeader const& frameHeader,
                             xml::SadmWriterOptions options) {
    xml::SadmXmlWriter writer(options);
    return writer.write(admDocument, frameHeader, stream);
  }  
}  // namespace adm
