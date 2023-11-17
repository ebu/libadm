#include <iostream>
#include <sstream>
#include "adm/parse_sadm.hpp"
#include "adm/write.hpp"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [SADM_XML_FILE]" << std::endl;
    exit(1);
  }

  std::string fileName = argv[1];
  auto header = adm::parseFrameHeader(fileName);
  auto document = adm::parseXml(fileName, header,
                                adm::xml::ParserOptions::recursive_node_search);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, document, header);
  std::cout << xmlStream.str();

  return 0;
}
