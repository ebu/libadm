#include <sstream>
#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;
namespace {
  std::string getXml(std::shared_ptr<Document>& doc) {
    std::stringstream ss;
    writeXml(ss, doc);
    return ss.str();
  }
}  // namespace

TEST_CASE("Write default ") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::DIRECT_SPEAKERS);
  doc->add(channelFormat);
  SECTION("Spherical DirectSpeaker block ") {
    auto blockFormat = AudioBlockFormatDirectSpeakers();
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_default_spherical_speaker"));
  }
  SECTION("Cartesian DirectSpeaker block") {
    auto blockFormat =
        AudioBlockFormatDirectSpeakers(CartesianSpeakerPosition());
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_default_cartesian_speaker"));
  }
}

TEST_CASE("Write fully specified ") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::DIRECT_SPEAKERS);
  doc->add(channelFormat);
  SECTION("Spherical DirectSpeaker block ") {
    auto blockFormat = AudioBlockFormatDirectSpeakers(
        SphericalSpeakerPosition{
            Azimuth(60.0f), AzimuthMin(58.0f), AzimuthMax(62.0f),
            Elevation(30.0f), ElevationMin(29.0f), ElevationMax(31.0f),
            Distance(0.5f), DistanceMin(0.4f), DistanceMax(0.6f),
            ScreenEdgeLock(HorizontalEdge("left"))},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}));
    blockFormat.add(SpeakerLabel("testLabel"));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_specified_spherical_speaker"));
  }

  SECTION("Cartesian DirectSpeaker block ") {
    auto blockFormat = AudioBlockFormatDirectSpeakers(
        CartesianSpeakerPosition{X(0.0f), XMin(-0.1f), XMax(0.1f), Y(0.0f),
                                 YMin(-0.1f), YMax(0.1f), Z(0.5f), ZMin(0.4f),
                                 ZMax(0.6f),
                                 ScreenEdgeLock(HorizontalEdge("left"))},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}));
    blockFormat.add(SpeakerLabel("testLabel"));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_specified_cartesian_speaker"));
  }
}

TEST_CASE("Write partially specified  ") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::OBJECTS);
  doc->add(channelFormat);
  SECTION("Spherical Object block ") {
    auto blockFormat = AudioBlockFormatObjects(
        SphericalPosition{Azimuth(60.0f), Elevation(30.0f)},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_partially_specified_spherical"));
  }

  SECTION("Cartesian Object block ") {
    auto blockFormat = AudioBlockFormatObjects(
        CartesianPosition{X(0.0f), Y(0.0f)}, Rtime(std::chrono::seconds{1}),
        Duration(std::chrono::seconds{1}));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_partially_specified_cartesian"));
  }
}

TEST_CASE("Write specified headphoneVirtualise") {
  auto doc = Document::create();

  // Objects
  {
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("TestObjects"), TypeDefinition::OBJECTS);
    doc->add(channelFormat);

    auto blockFormat = AudioBlockFormatObjects(
        CartesianPosition(X(0), Y(0), Z(0)),
        HeadphoneVirtualise(Bypass(false), DirectToReverberantRatio(30)));
    channelFormat->add(blockFormat);
  }

  // Direct Speakers
  {
    auto channelFormat =
        AudioChannelFormat::create(AudioChannelFormatName("TestDirectSpeakers"),
                                   TypeDefinition::DIRECT_SPEAKERS);
    doc->add(channelFormat);

    auto blockFormat = AudioBlockFormatDirectSpeakers(
        CartesianSpeakerPosition(),
        HeadphoneVirtualise(Bypass(false), DirectToReverberantRatio(30)));
    channelFormat->add(blockFormat);
  }

  // HOA
  {
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("TestHoa"), TypeDefinition::HOA);
    doc->add(channelFormat);

    auto blockFormat = AudioBlockFormatHoa(
        Order(1), Degree(0),
        HeadphoneVirtualise(Bypass(false), DirectToReverberantRatio(30)));
    channelFormat->add(blockFormat);
  }

  // Matrix
  {
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("TestMatrix"), TypeDefinition::MATRIX);
    doc->add(channelFormat);

    auto blockFormat = AudioBlockFormatMatrix(
        HeadphoneVirtualise(Bypass(false), DirectToReverberantRatio(30)));
    channelFormat->add(blockFormat);
  }

  // Binaural
  {
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("TestBinaural"), TypeDefinition::BINAURAL);
    doc->add(channelFormat);

    auto blockFormat = AudioBlockFormatBinaural(
        HeadphoneVirtualise(Bypass(false), DirectToReverberantRatio(30)));
    channelFormat->add(blockFormat);
  }

  auto xml = getXml(doc);
  REQUIRE(!xml.empty());

  CHECK_THAT(xml, EqualsXmlFile("write_specified_headphone_virtualise"));
}
