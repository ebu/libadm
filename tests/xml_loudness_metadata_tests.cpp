#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "adm/errors.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

void check_loudnessMetadata(const LoudnessMetadata &lm,
                            const std::string &method) {
  REQUIRE(lm.get<LoudnessMethod>() == method);
  REQUIRE(lm.get<LoudnessRecType>() == "EBU R128");
  REQUIRE(lm.get<LoudnessCorrectionType>() == "File-based");
  REQUIRE(lm.get<IntegratedLoudness>() == Approx(-23.f));
  REQUIRE(lm.get<LoudnessRange>() == Approx(10.f));
  REQUIRE(lm.get<MaxTruePeak>() == Approx(-2.3f));
  REQUIRE(lm.get<MaxMomentary>() == Approx(-19.f));
  REQUIRE(lm.get<MaxShortTerm>() == Approx(-21.2f));
  REQUIRE(lm.get<DialogueLoudness>() == Approx(-24.f));
}

template <typename Element>
void check_loudnessMetadatas(const Element &element) {
  REQUIRE(element->template has<LoudnessMetadatas>());
  LoudnessMetadatas lm = element->template get<LoudnessMetadatas>();
  REQUIRE(lm.size() == 2);
  check_loudnessMetadata(lm.at(0), "ITU-R BS.1770");
  check_loudnessMetadata(lm.at(1), "proprietary");
}

TEST_CASE("xml/loudness_metadata") {
  auto document = parseXml("xml_parser/loudness_metadata.xml");

  auto audioContent = document->lookup(parseAudioContentId("ACO_1001"));
  check_loudnessMetadatas(audioContent);

  auto audioProgramme = document->lookup(parseAudioProgrammeId("APR_1001"));
  check_loudnessMetadatas(audioProgramme);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("loudness_metadata"));
}
