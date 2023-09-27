#include <catch2/catch.hpp>
#include <adm/frame.hpp>
#include <adm/utilities/object_creation.hpp>
#include <chrono>
using namespace adm;

TEST_CASE("Frame deepCopy()") {
    using namespace std::chrono_literals;
    auto frameType = FrameType{"full"};
    auto frameStart = FrameStart{0s};
    auto frameDuration = FrameDuration{1s};
    auto frameFormatId = FrameFormatId(FrameFormatIdValue(1));
    auto header = FrameHeader{frameStart, frameDuration, frameType, frameFormatId};
    auto frame = Frame::create(header);
    SECTION("copies header") {
        auto copy = frame->deepCopy();
        auto copiedHeader = copy->frameHeader();
        auto frameFormat = copiedHeader.frameFormat();
        REQUIRE(frameFormat.get<FrameType>().get() == frameType.get());
        REQUIRE(frameFormat.get<FrameStart>().get() == frameStart.get());
        REQUIRE(frameFormat.get<FrameDuration>().get() == frameDuration.get());
        REQUIRE(frameFormat.get<FrameFormatId>().get<FrameFormatIdValue>() == frameFormatId.get<FrameFormatIdValue>());
    }
    SECTION("Modifying copy does not modify original header") {
        auto copy = frame->deepCopy();
        copy->frameHeader().frameFormat().set(FrameStart(500ms));
        REQUIRE(copy->frameHeader().frameFormat().get<FrameStart>().get() == 500ms);
        REQUIRE(frame->frameHeader().frameFormat().get<FrameStart>().get() == 0ms);

    }
    SECTION("copies document elements") {
        auto holder = addSimpleObjectTo(frame, "Test");
        auto copy = frame->deepCopy();
        auto objects = copy->getElements<adm::AudioObject>();
        REQUIRE(objects.size() == 1);
        REQUIRE(objects.front()->get<AudioObjectName>().get() == "Test");
    }
    SECTION("Modifying copy does not modify original elements") {
        REQUIRE(frame->getElements<AudioObject>().empty());
        auto copy = frame->deepCopy();
        REQUIRE(copy->getElements<AudioObject>().empty());
        addSimpleObjectTo(copy, "Test");
        REQUIRE(frame->getElements<AudioObject>().empty());
        REQUIRE(!copy->getElements<AudioObject>().empty());
    }
}