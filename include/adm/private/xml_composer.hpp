#pragma once
#include "adm/xml_writer.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "adm/private/rapidxml_wrapper.hpp"

namespace adm {
  class Document;
  namespace xml {

    using NodePtr = rapidxml::xml_node<>*;

    class XmlComposer {
     public:
      XmlComposer(WriterOptions options = WriterOptions::none);

      void compose(std::shared_ptr<const Document> document);
      std::ostream& writeXmlTo(std::ostream& stream);

     private:
      XmlDocument xmlDocument_;
      WriterOptions options_;
    };

  }  // namespace xml
}  // namespace adm
