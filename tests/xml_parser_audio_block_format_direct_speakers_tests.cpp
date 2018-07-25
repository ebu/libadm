#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"

TEST_CASE("xml_parser_audio_block_format_direct_speakers") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00010001\" "
        "audioChannelFormatName=\"FrontLeft\" "
        "typeLabel=\"0001\" "
        "typeDefinition=\"DirectSpeakers\">"
        "<audioBlockFormat audioBlockFormatID=\"AB_00010001_00000001\">"
        "<speakerLabel>M+30</speakerLabel>"
        "<position coordinate=\"azimuth\">30.0</position>"
        "<position coordinate=\"elevation\">0.0</position>"
        "<position coordinate=\"distance\">1.0</position>"
        "</audioBlockFormat>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto channelFormats = document->getElements<AudioChannelFormat>();
    auto channelFormat = *channelFormats.begin();
    REQUIRE(channelFormat->get<AudioChannelFormatId>()
                .get<AudioChannelFormatIdValue>() == 0x0001u);
    REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(channelFormat->get<AudioChannelFormatName>() == "FrontLeft");
    REQUIRE(channelFormat->get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    auto firstBlockFormat =
        *(channelFormat->getElements<AudioBlockFormatDirectSpeakers>().begin());

    auto speakerLabels = firstBlockFormat.get<SpeakerLabels>();
    REQUIRE(*speakerLabels.begin() == "M+30");
    REQUIRE(firstBlockFormat.get<SpeakerPosition>().get<Azimuth>() == 30.0f);
    REQUIRE(firstBlockFormat.get<SpeakerPosition>().get<Elevation>() == 0.0f);
  }
}
