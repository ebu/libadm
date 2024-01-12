#include <sstream>
#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"
#include "adm/serial.hpp"

using namespace adm;
namespace {
  std::string getXml(std::shared_ptr<Document>& doc) {
    std::stringstream ss;
    writeXml(ss, doc);
    return ss.str();
  }
}  // namespace

TEST_CASE("Write default DirectSpeakers block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::DIRECT_SPEAKERS);
  doc->add(channelFormat);
  SECTION("Spherical") {
    auto blockFormat = AudioBlockFormatDirectSpeakers();
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_default_spherical_speaker"));
  }
  SECTION("Cartesian") {
    auto blockFormat =
        AudioBlockFormatDirectSpeakers(CartesianSpeakerPosition());
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_default_cartesian_speaker"));
  }
}

TEST_CASE("Write fully specified DirectSpeakers block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::DIRECT_SPEAKERS);
  doc->add(channelFormat);
  SECTION("Spherical") {
    auto blockFormat = AudioBlockFormatDirectSpeakers(
        SphericalSpeakerPosition{
            Azimuth(60.0f), AzimuthMin(58.0f), AzimuthMax(62.0f),
            Elevation(30.0f), ElevationMin(29.0f), ElevationMax(31.0f),
            Distance(0.5f), DistanceMin(0.4f), DistanceMax(0.6f),
            ScreenEdgeLock(HorizontalEdge("left"))},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}),
        Gain::fromLinear(0.5), Importance{5}, HeadLocked{true});
    blockFormat.add(SpeakerLabel("testLabel"));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_specified_spherical_speaker"));
  }

  SECTION("Cartesian") {
    auto blockFormat = AudioBlockFormatDirectSpeakers(
        CartesianSpeakerPosition{X(0.0f), XMin(-0.1f), XMax(0.1f), Y(0.0f),
                                 YMin(-0.1f), YMax(0.1f), Z(0.5f), ZMin(0.4f),
                                 ZMax(0.6f),
                                 ScreenEdgeLock(HorizontalEdge("left"))},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}),
        Gain::fromLinear(0.5), Importance{5}, HeadLocked{true});
    blockFormat.add(SpeakerLabel("testLabel"));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_specified_cartesian_speaker"));
  }
}

TEST_CASE("Write partially specified Objects positions") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::OBJECTS);
  doc->add(channelFormat);
  SECTION("Spherical Object block") {
    auto blockFormat = AudioBlockFormatObjects(
        SphericalPosition{Azimuth(60.0f), Elevation(30.0f)},
        Rtime(std::chrono::seconds{1}), Duration(std::chrono::seconds{1}));
    channelFormat->add(blockFormat);
    auto xml = getXml(doc);
    REQUIRE(!xml.empty());
    CHECK_THAT(xml, EqualsXmlFile("write_partially_specified_spherical"));
  }

  SECTION("Cartesian Object block") {
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

  auto xml = getXml(doc);
  REQUIRE(!xml.empty());

  CHECK_THAT(xml, EqualsXmlFile("write_specified_headphone_virtualise"));
}

TEST_CASE("write default HOA block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::HOA);
  doc->add(channelFormat);
  auto blockFormat = AudioBlockFormatHoa{Order{0}, Degree{0}};
  channelFormat->add(blockFormat);

  auto xml = getXml(doc);
  CHECK_THAT(xml, EqualsXmlFile("write_default_HOA_block"));
}

TEST_CASE("write specified HOA block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::HOA);
  doc->add(channelFormat);
  auto blockFormat = AudioBlockFormatHoa{Order{2},
                                         Degree{1},
                                         Rtime{std::chrono::seconds(0)},
                                         Duration{std::chrono::seconds(1)},
                                         NfcRefDist{2.0},
                                         ScreenRef{true},
                                         Normalization{"N3D"},
                                         Gain::fromLinear(0.5),
                                         Importance{5},
                                         HeadLocked{true},
                                         Equation{"do not use equation!"}};
  channelFormat->add(blockFormat);

  auto xml = getXml(doc);
  CHECK_THAT(xml, EqualsXmlFile("write_specified_HOA_block"));
}

TEST_CASE("write default Binaural block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::BINAURAL);
  doc->add(channelFormat);
  auto blockFormat = AudioBlockFormatBinaural{};
  channelFormat->add(blockFormat);

  auto xml = getXml(doc);
  CHECK_THAT(xml, EqualsXmlFile("write_default_binaural_block"));
}

TEST_CASE("write specified Binaural block") {
  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::BINAURAL);
  doc->add(channelFormat);
  auto blockFormat = AudioBlockFormatBinaural{
      Rtime{std::chrono::seconds(0)}, Duration{std::chrono::seconds(1)},
      Gain::fromLinear(0.5), Importance{5}};
  channelFormat->add(blockFormat);

  auto xml = getXml(doc);
  CHECK_THAT(xml, EqualsXmlFile("write_specified_binaural_block"));
}

TEST_CASE("write specified Binaural block sadm") {
  using namespace std::chrono_literals;

  auto doc = Document::create();
  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Test"), TypeDefinition::BINAURAL);
  doc->add(channelFormat);
  auto blockFormat = AudioBlockFormatBinaural{
      Rtime{std::chrono::seconds(0)}, Duration{std::chrono::seconds(1)},
      Gain::fromLinear(0.5), Importance{5}};
  channelFormat->add(blockFormat);

  FrameFormat format{FrameFormatId{FrameIndex{1}}, Start{0s}, Duration{1s},
                     FrameType::FULL, TimeReference::LOCAL};

  std::stringstream ss;
  writeXml(ss, doc, FrameHeader{format});
  CHECK_THAT(ss.str(), EqualsXmlFile("write_specified_binaural_block_sadm"));
}

namespace {
  //clang-format off
  constexpr const char* LOCAL_XML = R"(<?xml version="1.0" encoding="utf-8"?>
<frame version="ITU-R_BS.2125-1">
	<frameHeader>
		<frameFormat frameFormatID="FF_00000001" start="00:00:00.00000" duration="00:00:01.00000" type="full" timeReference="local"/>
	</frameHeader>
	<audioFormatExtended>
		<audioChannelFormat audioChannelFormatID="AC_00031001" audioChannelFormatName="Objects Format" typeLabel="0003" typeDefinition="Objects">
			<audioBlockFormat audioBlockFormatID="AB_00031001_00000001" lstart="00:00:01.00000" lduration="00:00:02.00000">
				<position coordinate="X">0.000000</position>
				<position coordinate="Y">1.000000</position>
				<cartesian>1</cartesian>
			</audioBlockFormat>
		</audioChannelFormat>
	</audioFormatExtended>
</frame>

)";

  constexpr const char* TOTAL_XML = R"(<?xml version="1.0" encoding="utf-8"?>
<frame version="ITU-R_BS.2125-1">
	<frameHeader>
		<frameFormat frameFormatID="FF_00000001" start="00:00:00.00000" duration="00:00:01.00000" type="full" timeReference="total"/>
	</frameHeader>
	<audioFormatExtended>
		<audioChannelFormat audioChannelFormatID="AC_00031001" audioChannelFormatName="Objects Format" typeLabel="0003" typeDefinition="Objects">
			<audioBlockFormat audioBlockFormatID="AB_00031001_00000001" rtime="00:00:01.00000" duration="00:00:02.00000">
				<position coordinate="X">0.000000</position>
				<position coordinate="Y">1.000000</position>
				<cartesian>1</cartesian>
			</audioBlockFormat>
		</audioChannelFormat>
	</audioFormatExtended>
</frame>

)";

  //clang-format on
}  // namespace

TEST_CASE("Time reference writing") {
  using namespace std::chrono_literals;
  Rtime start{1s};
  Duration duration{2s};
  CartesianPosition position{X{0}, Y{1}};
  auto document = Document::create();
  //  auto holder = addSimpleObjectTo(document, "test");
  FrameFormat format{FrameFormatId{FrameIndex{1}}, Start{0s}, Duration{1s},
                     FrameType::FULL};
  SECTION("Objects, total time") {
    format.set(TimeReference::TOTAL);
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName{"Objects Format"}, TypeDefinition::OBJECTS);
    channelFormat->add(AudioBlockFormatObjects{position, start, duration});
    document->add(channelFormat);
    std::stringstream ss;
    writeXml(ss, document, FrameHeader{format});
    REQUIRE(ss.str() == TOTAL_XML);
  }
  SECTION("Objects, local time") {
    format.set(TimeReference::LOCAL);
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName{"Objects Format"}, TypeDefinition::OBJECTS);
    channelFormat->add(AudioBlockFormatObjects{position, start, duration});
    document->add(channelFormat);
    std::stringstream ss;
    writeXml(ss, document, FrameHeader{format});
    REQUIRE(ss.str() == LOCAL_XML);
  }
}