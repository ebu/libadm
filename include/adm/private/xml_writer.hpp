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
      std::ostream& write(std::shared_ptr<const Frame> frame,
                          std::ostream& stream);
                          
     private:
      WriterOptions options_;
    };

  }  // namespace xml
}  // namespace adm
