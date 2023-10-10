#pragma once
#include "adm/write.hpp"

namespace adm {
  class Document;
  namespace xml {

    class XmlWriter {
     public:
      explicit XmlWriter(WriterOptions options = WriterOptions::none);

      std::ostream& write(std::shared_ptr<const Document> document,
                          std::ostream& stream);
     private:
      WriterOptions options_;
    };

    class SadmXmlWriter {
     public:
      SadmXmlWriter(SadmWriterOptions options = SadmWriterOptions::none);
      std::ostream& write(std::shared_ptr<const Document> document,
                          FrameHeader const& header,
                          std::ostream& stream);
     private:
      SadmWriterOptions options_;
    };


  }  // namespace xml
}  // namespace adm
