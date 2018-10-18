#include <iostream>
#include <sstream>
#include "adm/common_definitions.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [XML_FILE]" << std::endl;
    exit(1);
  }
  auto admDocument = adm::parseXml(argv[1]);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();

  return 0;
}
