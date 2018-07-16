#include <iostream>
#include <sstream>
#include "adm/common_definitions.hpp"
#include "adm/xml_reader.hpp"
#include "adm/xml_writer.hpp"
#include "adm/segmenter.hpp"
#include "adm/combiner.hpp"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [XML_FILE]" << std::endl;
    exit(1);
  }
  auto admDocument = adm::parseXml(argv[1]);

  adm::Segmenter segmenter(admDocument);
  adm::Combiner combiner;
  for (unsigned int i = 0; i < 206; ++i) {
    auto frame = segmenter.getFrame(
        adm::SegmentStart(std::chrono::seconds(i)),
        adm::SegmentDuration(std::chrono::seconds(1)));
    combiner.push(frame);
  }

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, combiner.getDocument());
  std::cout << xmlStream.str();

  return 0;
}
