#include <iostream>
#include <sstream>
#include <fstream>
#include "adm/common_definitions.hpp"
#include "adm/parse_sadm.hpp"
#include "adm/write.hpp"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [SADM_XML_FILE]" << std::endl;
    exit(1);
  }
  
  // We get common definitions once before we get possibly many S-ADM frames
  auto commonDefinitions = adm::getSadmCommonDefinitions();

  std::fstream sadm_file(argv[1], std::ios::in);
	if (sadm_file.is_open()) {
    auto frame = adm::parseSadmXmlWithCommonDefs(sadm_file, commonDefinitions, 
                                   adm::xml::ParserOptions::recursive_node_search);

    // write XML data to stdout
    std::stringstream xmlStream;
    writeXmlSadm(xmlStream, frame);
    std::cout << xmlStream.str();
  } else {
    std::cerr << "Can't open file" << std::endl;
  }

  return 0;
}
