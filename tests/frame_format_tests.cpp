#include <sstream>
#include <fstream>
#include <catch2/catch.hpp>
#include <adm/serial/frame_format.hpp>
#include <adm/serial/frame_header.hpp>
#include <adm/document.hpp>
#include <adm/write.hpp>
#include <adm/parse_sadm.hpp>
#include <adm/serial/changed_ids.hpp>

using namespace adm;
using namespace std::chrono_literals;
TEST_CASE("Invalid format id parsing") {
  CHECK_THROWS(parseFrameFormatId("FF_00000000"));  // bad value
  CHECK_THROWS(parseFrameFormatId("FA_00000001"));  // bad prefix
  CHECK_THROWS(parseFrameFormatId("FF_0000000"));  // too short
  CHECK_THROWS(parseFrameFormatId("FF_000000001"));  // too long
  CHECK_THROWS(parseFrameFormatId("FA_00000001_01"));  // bad prefix
  CHECK_THROWS(parseFrameFormatId("FF_00000001_00"));  // bad value
  CHECK_THROWS(parseFrameFormatId("FF_00000001001"));  // no divider
  CHECK_THROWS(parseFrameFormatId("FF_00000001_0"));  // postfix too short
  CHECK_THROWS(parseFrameFormatId("FF_00000001_"));  // postfix too short
  CHECK_THROWS(parseFrameFormatId("FF_00000001_001"));  // postfix too long
  CHECK_THROWS(parseFrameFormatId("FF_000000g1"));  // bad hex
}

TEST_CASE("Short format id parsing") {
  CHECK(parseFrameFormatId("FF_00000001").get<FrameIndex>() == 1);
  CHECK(parseFrameFormatId("FF_000000FF").get<FrameIndex>() == 255);
  CHECK(parseFrameFormatId("FF_000000ff").get<FrameIndex>() == 255);
  CHECK(parseFrameFormatId("FF_FFFFFFFF").get<FrameIndex>() == 0xFFFFFFFF);
  CHECK(!parseFrameFormatId("FF_00000001").has<ChunkIndex>());
}

TEST_CASE("Long format id parsing") {
  CHECK(parseFrameFormatId("FF_00000001_01").get<FrameIndex>() == 1);
  CHECK(parseFrameFormatId("FF_000000FF_92").get<FrameIndex>() == 255);
  CHECK(parseFrameFormatId("FF_FFFFFFFF_FF").get<FrameIndex>() == 0xFFFFFFFF);
  REQUIRE(parseFrameFormatId("FF_00000001_01").has<ChunkIndex>());
  CHECK(parseFrameFormatId("FF_00000001_01").get<ChunkIndex>() == 1);
  CHECK(parseFrameFormatId("FF_00000001_02").get<ChunkIndex>() == 2);
  CHECK(parseFrameFormatId("FF_00000001_FF").get<ChunkIndex>() == 255);
  CHECK(parseFrameFormatId("FF_FFFFFFFF_03").get<ChunkIndex>() == 3);
}

TEST_CASE("id formatting") {
  CHECK(formatId(FrameFormatId{FrameIndex{1}}) == "FF_00000001");
  CHECK(formatId(FrameFormatId{FrameIndex{2}}) == "FF_00000002");
  CHECK(formatId(FrameFormatId{FrameIndex{1}, ChunkIndex{1}}) ==
        "FF_00000001_01");
  CHECK(formatId(FrameFormatId{FrameIndex{1}, ChunkIndex{255}}) ==
        "FF_00000001_ff");
  CHECK(formatId(FrameFormatId{FrameIndex{255}}) == "FF_000000ff");
  CHECK(formatId(FrameFormatId{FrameIndex{0xFFFFFFFF}, ChunkIndex{0xFF}}) ==
        "FF_ffffffff_ff");
}

TEST_CASE("id equality") {
  auto index_four = FrameIndex{4};
  auto index_nine = FrameIndex{9};
  auto chunk_two = ChunkIndex{2};
  auto chunk_one = ChunkIndex{1};
  SECTION("simple identical ids are equal") {
    auto four = FrameFormatId{index_four};
    auto four_copy = FrameFormatId{index_four};
    REQUIRE(four == four_copy);
  }
  SECTION("simple differing ids are not equal") {
    auto four = FrameFormatId{index_four};
    auto nine = FrameFormatId{index_nine};
    CHECK(!(four == nine));
    CHECK(four != nine);
  }
  SECTION("long identical ids are equal") {
    auto four_two = FrameFormatId{index_four, chunk_two};
    auto four_two_copy = FrameFormatId{index_four, chunk_two};
    REQUIRE(four_two == four_two_copy);
  }
  SECTION("long ids with same index but different chunk not equal") {
    auto four_two = FrameFormatId{index_four, chunk_two};
    auto four_one = FrameFormatId{index_four, chunk_one};
    CHECK(!(four_two == four_one));
    CHECK(four_two != four_one);
  }
  SECTION("long ids with same chunk but different index not equal") {
    auto four_two = FrameFormatId{index_four, chunk_two};
    auto nine_two = FrameFormatId{index_nine, chunk_two};
    CHECK(!(four_two == nine_two));
    CHECK(four_two != nine_two);
  }
}

static constexpr char const* NUM_METADATA_ATTR_XML =
    // clang-format off
R"(<?xml version="1.0" encoding="utf-8"?>
<frame version="ITU-R_BS.2125-1">
	<frameHeader>
		<frameFormat frameFormatID="FF_00000001" start="00:00:00.00000" duration="00:00:01.00000" type="divided" numMetadataChunks="3"/>
	</frameHeader>
	<audioFormatExtended/>
</frame>

)";

static constexpr char const* COUNT_TO_SAME_CHUNK_XML =
R"(<?xml version="1.0" encoding="utf-8"?>
<frame version="ITU-R_BS.2125-1">
	<frameHeader>
		<frameFormat frameFormatID="FF_00000001" start="00:00:00.00000" duration="00:00:01.00000" type="divided" countToSameChunk="3"/>
	</frameHeader>
	<audioFormatExtended/>
</frame>

)";
// clang-format on

TEST_CASE("NumMetadataChunks correctly written as attribute") {
  FrameHeader header{FrameFormat{FrameFormatId{FrameIndex{1}}, FrameStart{0ms},
                                 FrameDuration{1s}, FrameType("divided"),
                                 NumMetadataChunks{3}}};
  auto document = Document::create();
  std::stringstream out;
  writeXml(out, document, header);
  REQUIRE(out.str() == NUM_METADATA_ATTR_XML);
}

TEST_CASE("NumMetadataChunks correctly parsed from attribute") {
  auto ss = std::stringstream(NUM_METADATA_ATTR_XML);
  auto header = parseFrameHeader(ss);
  auto format = header.get<FrameFormat>();
  REQUIRE(format.has<NumMetadataChunks>());
  auto numChunks = format.get<NumMetadataChunks>();
  REQUIRE(numChunks == 3);
}

TEST_CASE("CountToSameChunk correctly written as attribute") {
  FrameHeader header{FrameFormat{FrameFormatId{FrameIndex{1}}, FrameStart{0ms},
                                 FrameDuration{1s}, FrameType("divided"),
                                 CountToSameChunk{3}}};
  auto document = Document::create();
  std::stringstream out;
  writeXml(out, document, header);
  REQUIRE(out.str() == COUNT_TO_SAME_CHUNK_XML);
}

TEST_CASE("CountToSameChunk correctly parsed from attribute") {
  auto ss = std::stringstream(COUNT_TO_SAME_CHUNK_XML);
  auto header = parseFrameHeader(ss);
  auto format = header.get<FrameFormat>();
  REQUIRE(format.has<CountToSameChunk>());
  auto numChunks = format.get<CountToSameChunk>();
  REQUIRE(numChunks == 3);
}

//clang-format off
static constexpr const char* CHANGED_IDS_A2_2 =
    R"(<?xml version="1.0" encoding="utf-8"?>
<frame version="ITU-R_BS.2125-1">
	<frameHeader>
		<frameFormat frameFormatID="FF_00000004" start="00:00:03.00000" duration="00:00:01.00000" type="full">
			<changedIDs>
				<audioChannelFormatIDRef status="changed">AC_00031001</audioChannelFormatIDRef>
				<audioChannelFormatIDRef status="expired">AC_00031002</audioChannelFormatIDRef>
				<audioChannelFormatIDRef status="extended">AC_00031003</audioChannelFormatIDRef>
			</changedIDs>
		</frameFormat>
	</frameHeader>
	<audioFormatExtended/>
</frame>

)";
//clang-format on

TEST_CASE("ChangedIDs correctly written - BS2125-1 A2.2, 2nd example") {
  FrameHeader header(FrameFormat{
      FrameFormatId{FrameIndex{4}}, FrameStart{3s}, FrameDuration{1s},
      FrameType{"full"},
      ChangedIds{AudioChannelFormatIdRefs{
          {parseAudioChannelFormatId("AC_00031001"), Status("changed")},
          {parseAudioChannelFormatId("AC_00031002"), Status("expired")},
          {parseAudioChannelFormatId("AC_00031003"), Status("extended")}}}});
  std::stringstream ss("changedids.xml");
  writeXml(ss, adm::Document::create(), header);
  REQUIRE(ss.str() == CHANGED_IDS_A2_2);
}

TEST_CASE("ChangedIDs correctly parsed - BS2125-1 A2.2, 2nd example") {
  std::stringstream ss(CHANGED_IDS_A2_2);
  auto header = parseFrameHeader(ss);
  auto format = header.get<FrameFormat>();
  REQUIRE(format.has<ChangedIds>());
  auto ids = format.get<ChangedIds>();
  auto channelFormatRefs = ids.get<AudioChannelFormatIdRefs>();
  REQUIRE(channelFormatRefs.size() == 3);
  REQUIRE(formatId(channelFormatRefs[0].get<AudioChannelFormatId>()) ==
          "AC_00031001");
  REQUIRE(channelFormatRefs[0].get<Status>().get() == "changed");
  REQUIRE(formatId(channelFormatRefs[1].get<AudioChannelFormatId>()) ==
          "AC_00031002");
  REQUIRE(channelFormatRefs[1].get<Status>().get() == "expired");
  REQUIRE(formatId(channelFormatRefs[2].get<AudioChannelFormatId>()) ==
          "AC_00031003");
  REQUIRE(channelFormatRefs[2].get<Status>().get() == "extended");
}
