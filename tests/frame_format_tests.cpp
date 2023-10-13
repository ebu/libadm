#include <catch2/catch.hpp>
#include <adm/serial/frame_format.hpp>

using namespace adm;
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