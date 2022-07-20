#include "adm/common_definitions.hpp"
#include "resources.hpp"
#include "adm/private/xml_parser.hpp"
#include "adm/utilities/copy.hpp"
#include <iostream>
#include <iomanip>

namespace adm {

  const std::map<std::string, adm::AudioPackFormatId>
  audioPackFormatLookupTable() {
    return std::map<std::string, adm::AudioPackFormatId>{
        {"0+1+0", adm::parseAudioPackFormatId("AP_00010001")},
        {"0+2+0", adm::parseAudioPackFormatId("AP_00010002")},
        {"0+5+0", adm::parseAudioPackFormatId("AP_00010003")},
        {"2+5+0", adm::parseAudioPackFormatId("AP_00010004")},
        {"4+5+0", adm::parseAudioPackFormatId("AP_00010005")},
        {"4+5+1", adm::parseAudioPackFormatId("AP_00010010")},
        {"3+7+0", adm::parseAudioPackFormatId("AP_00010007")},
        {"4+9+0", adm::parseAudioPackFormatId("AP_00010008")},
        {"9+10+3", adm::parseAudioPackFormatId("AP_00010009")},
        {"0+7+0", adm::parseAudioPackFormatId("AP_0001000f")},
        {"4+7+0", adm::parseAudioPackFormatId("AP_00010017")},
        {"SN3D-Order1-3D", adm::parseAudioPackFormatId("AP_00040001")},
        {"SN3D-Order2-3D", adm::parseAudioPackFormatId("AP_00040002")},
        {"SN3D-Order3-3D", adm::parseAudioPackFormatId("AP_00040003")},
        {"SN3D-Order4-3D", adm::parseAudioPackFormatId("AP_00040004")},
        {"SN3D-Order5-3D", adm::parseAudioPackFormatId("AP_00040005")},
        {"SN3D-Order6-3D", adm::parseAudioPackFormatId("AP_00040006")},
        {"N3D-Order1-3D", adm::parseAudioPackFormatId("AP_00040011")},
        {"N3D-Order2-3D", adm::parseAudioPackFormatId("AP_00040012")},
        {"N3D-Order3-3D", adm::parseAudioPackFormatId("AP_00040013")},
        {"N3D-Order4-3D", adm::parseAudioPackFormatId("AP_00040014")},
        {"N3D-Order5-3D", adm::parseAudioPackFormatId("AP_00040015")},
        {"N3D-Order6-3D", adm::parseAudioPackFormatId("AP_00040016")},
        {"FuMa-Order1-3D", adm::parseAudioPackFormatId("AP_00040021")},
        {"FuMa-Order2-3D", adm::parseAudioPackFormatId("AP_00040022")},
        {"FuMa-Order3-3D", adm::parseAudioPackFormatId("AP_00040023")},
        {"FuMa-Order4-3D", adm::parseAudioPackFormatId("AP_00040024")},
        {"FuMa-Order5-3D", adm::parseAudioPackFormatId("AP_00040025")},
        {"FuMa-Order6-3D", adm::parseAudioPackFormatId("AP_00040026")},
    };
  };

  const std::map<std::string, adm::AudioTrackFormatId>
  audioTrackFormatLookupTable() {
    return std::map<std::string, adm::AudioTrackFormatId>{
        {"M+000", adm::parseAudioTrackFormatId("AT_00010003_01")},
        {"M+022", adm::parseAudioTrackFormatId("AT_00010007_01")},
        {"M-022", adm::parseAudioTrackFormatId("AT_00010008_01")},
        {"M+SC", adm::parseAudioTrackFormatId("AT_00010024_01")},
        {"M-SC", adm::parseAudioTrackFormatId("AT_00010025_01")},
        {"M+030", adm::parseAudioTrackFormatId("AT_00010001_01")},
        {"M-030", adm::parseAudioTrackFormatId("AT_00010002_01")},
        {"M+045", adm::parseAudioTrackFormatId("AT_00010026_01")},
        {"M-045", adm::parseAudioTrackFormatId("AT_00010027_01")},
        {"M+060", adm::parseAudioTrackFormatId("AT_00010018_01")},
        {"M-060", adm::parseAudioTrackFormatId("AT_00010019_01")},
        {"M+090", adm::parseAudioTrackFormatId("AT_0001000a_01")},
        {"M-090", adm::parseAudioTrackFormatId("AT_0001000b_01")},
        {"M+110", adm::parseAudioTrackFormatId("AT_00010005_01")},
        {"M-110", adm::parseAudioTrackFormatId("AT_00010006_01")},
        {"M+135", adm::parseAudioTrackFormatId("AT_0001001c_01")},
        {"M-135", adm::parseAudioTrackFormatId("AT_0001001d_01")},
        {"M+180", adm::parseAudioTrackFormatId("AT_00010009_01")},
        {"U+000", adm::parseAudioTrackFormatId("AT_0001000e_01")},
        // {"U+022", adm::parseAudioTrackFormatId("")},
        // {"U-022", adm::parseAudioTrackFormatId("")},
        {"U+030", adm::parseAudioTrackFormatId("AT_0001000d_01")},
        {"U-030", adm::parseAudioTrackFormatId("AT_0001000f_01")},
        {"U+045", adm::parseAudioTrackFormatId("AT_00010022_01")},
        {"U-045", adm::parseAudioTrackFormatId("AT_00010023_01")},
        // {"U+060", adm::parseAudioTrackFormatId("")},
        // {"U-060", adm::parseAudioTrackFormatId("")},
        {"U+090", adm::parseAudioTrackFormatId("AT_00010013_01")},
        {"U-090", adm::parseAudioTrackFormatId("AT_00010014_01")},
        {"U+110", adm::parseAudioTrackFormatId("AT_00010010_01")},
        {"U-110", adm::parseAudioTrackFormatId("AT_00010012_01")},
        {"U+135", adm::parseAudioTrackFormatId("AT_0001001e_01")},
        {"U-135", adm::parseAudioTrackFormatId("AT_0001001f_01")},
        {"U+180", adm::parseAudioTrackFormatId("AT_00010011_01")},
        {"UH+180", adm::parseAudioTrackFormatId("AT_00010028_01")},
        {"T+000", adm::parseAudioTrackFormatId("AT_0001000c_01")},
        {"B+000", adm::parseAudioTrackFormatId("AT_00010015_01")},
        // {"B+022", adm::parseAudioTrackFormatId("")},
        // {"B-022", adm::parseAudioTrackFormatId("")},
        // {"B+030", adm::parseAudioTrackFormatId("")},
        // {"B-030", adm::parseAudioTrackFormatId("")},
        {"B+045", adm::parseAudioTrackFormatId("AT_00010016_01")},
        {"B-045", adm::parseAudioTrackFormatId("AT_00010017_01")},
        // {"B+060", adm::parseAudioTrackFormatId("")},
        // {"B-060", adm::parseAudioTrackFormatId("")},
        // {"B+090", adm::parseAudioTrackFormatId("")},
        // {"B-090", adm::parseAudioTrackFormatId("")},
        // {"B+110", adm::parseAudioTrackFormatId("")},
        // {"B-110", adm::parseAudioTrackFormatId("")},
        // {"B+135", adm::parseAudioTrackFormatId("")},
        // {"B-135", adm::parseAudioTrackFormatId("")},
        // {"B+180", adm::parseAudioTrackFormatId("")},
        {"LFE", adm::parseAudioTrackFormatId("AT_00010004_01")},
        {"LFE1", adm::parseAudioTrackFormatId("AT_00010020_01")},
        {"LFE2", adm::parseAudioTrackFormatId("AT_00010021_01")}};
  };

  const std::map<std::string, std::vector<std::string>>
  speakerLabelsLookupTable() {
    return std::map<std::string, std::vector<std::string>>{
        {"0+1+0", {"M+000"}},
        {"0+2+0", {"M+030", "M-030"}},
        {"0+5+0", {"M+030", "M-030", "M+000", "LFE", "M+110", "M-110"}},
        {"2+5+0",
         {"M+030", "M-030", "M+000", "LFE", "M+110", "M-110", "U+030",
          "U-030"}},
        {"4+5+0",
         {"M+030", "M-030", "M+000", "LFE", "M+110", "M-110", "U+030", "U-030",
          "U+110", "U-110"}},
        {"4+5+1",
         {"M+030", "M-030", "M+000", "LFE", "M+110", "M-110", "U+030", "U-030",
          "U+110", "U-110", "B+000"}},
        {"3+7+0",
         {"M+000", "M+030", "M-030", "U+045", "U-045", "M+090", "M-090",
          "M+135", "M-135", "UH+180", "LFE1", "LFE2"}},
        {"4+9+0",
         {"M+030", "M-030", "M+000", "LFE", "M+090", "M-090", "M+135", "M-135",
          "U+045", "U-045", "U+135", "U-135", "M+SC", "M-SC"}},
        {"9+10+3", {"M+060", "M-060", "M+000", "LFE1",  "M+135", "M-135",
                    "M+030", "M-030", "M+180", "LFE2",  "M+090", "M-090",
                    "U+045", "U-045", "U+000", "T+000", "U+135", "U-135",
                    "U+090", "U-090", "U+180", "B+000", "B+045", "B-045"}},
        {"0+7+0",
         {"M+030", "M-030", "M+000", "LFE", "M+090", "M-090", "M+135",
          "M-135"}},
        {"4+7+0",
         {"M+030", "M-030", "M+000", "LFE", "M+090", "M-090", "M+135", "M-135",
          "U+045", "U-045", "U+135", "U-135"}}};
  }

  const adm::AudioTrackFormatId audioTrackFormatHoaLookup(
      int order, int degree, std::string normalization) {
    int normalizationTypeValue;
    if (normalization == "SN3D") {
      normalizationTypeValue = 0;
    } else if (normalization == "N3D") {
      normalizationTypeValue = 1;
    } else if (normalization == "FuMa") {
      normalizationTypeValue = 2;
    } else {
      throw std::invalid_argument("Not a supported normalization value.");
    }
    // ACN starts from 0, while AudioTrackFormatId starts from 1
    int trackFormatIdValue =
        order * order + order + degree + 1 + normalizationTypeValue * 0x100;
    return adm::AudioTrackFormatId(TypeDefinition::HOA,
                                   AudioTrackFormatIdValue(trackFormatIdValue),
                                   AudioTrackFormatIdCounter(1));
  }

  std::shared_ptr<Document> getCommonDefinitions() {
    std::stringstream commonDefinitions;
    getEmbeddedFile("common_definitions.xml", commonDefinitions);

    std::shared_ptr<Document> destDocument = Document::create();

    xml::XmlParser parser(destDocument,
                          xml::ParserOptions::recursive_node_search);
    parser.parseStream(commonDefinitions);

    return destDocument;
  }

  void addCommonDefinitionsTo(std::shared_ptr<Document> document) {
    deepCopyTo(getCommonDefinitions(), document);
  }

}  // namespace adm
