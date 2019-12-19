#include <catch2/catch.hpp>
#include "adm/common_definitions.hpp"

TEST_CASE("basic_document") {
  using namespace adm;

  // create a basic document
  { auto commonDefinitions = getCommonDefinitions(); }
}

TEST_CASE("Parse HOA") {
    using namespace adm;


    auto admDocument = Document::create();
    addCommonDefinitionsTo(admDocument);

    auto packFormatLookup = audioPackFormatLookupTable();
    auto trackFormatLookup = audioTrackFormatLookupTable();
    auto sn3d_order1_3d = admDocument->lookup(packFormatLookup.at("SN3D-Order1-3D"));

    auto name = sn3d_order1_3d.get()->get<AudioPackFormatName>();
    REQUIRE(name == "3D_order1_SN3D_ACN");

    auto type = sn3d_order1_3d.get()->get<TypeDescriptor>();
    REQUIRE(type == 4);

    auto fuma_order2_3d = admDocument->lookup(packFormatLookup.at("FuMa-Order2-3D"));

    name = fuma_order2_3d.get()->get<AudioPackFormatName>();
    REQUIRE(name == "3D_order2_FuMa");

    type = fuma_order2_3d.get()->get<TypeDescriptor>();
    REQUIRE(type == 4);
}

TEST_CASE("HOA Track Lookup") {
    using namespace adm;

    auto admDocument = adm::Document::create();
    addCommonDefinitionsTo(admDocument);

    auto audioTrackFormatId_SN3D = audioTrackFormatHoaLookup(0, 0, "SN3D");
    auto label_SN3D = formatId(audioTrackFormatId_SN3D);
    REQUIRE(label_SN3D == "AT_00040001_01");

    auto audioTrackFormatId_N3D = audioTrackFormatHoaLookup(7, 6, "N3D");
    auto label_N3D = formatId(audioTrackFormatId_N3D);
    REQUIRE(label_N3D == "AT_0004013f_01");

    auto audioTrackFormatId_FuMa = audioTrackFormatHoaLookup(3, -1, "FuMa");
    auto label_FuMa = formatId(audioTrackFormatId_FuMa);
    REQUIRE(label_FuMa == "AT_0004020c_01");
}
