#pragma once
#include "adm/write.hpp"
#include <memory>

namespace adm {
  class Document;
  class AudioProgramme;

  namespace xml {

    class XmlNode;

    class XmlWriter {
     public:
      explicit XmlWriter(WriterOptions options = WriterOptions::none);

      std::ostream& write(std::shared_ptr<const Document> document,
                          std::ostream& stream);

     protected:
      virtual void formatAudioProgramme(
          XmlNode& node, const std::shared_ptr<const AudioProgramme> programme);

     private:
      WriterOptions options_;
    };

  }  // namespace xml
}  // namespace adm
