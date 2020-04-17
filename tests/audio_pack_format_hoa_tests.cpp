#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_pack_format_hoa.hpp"

TEST_CASE("audio_pack_format_hoa_without_extra_param") {
  using namespace adm;

  auto packFormatHoa =
      AudioPackFormatHoa::create(AudioPackFormatName("MyPackFormatHoa"));
  packFormatHoa->set(
      AudioPackFormatId(TypeDefinition::HOA, AudioPackFormatIdValue(1)));
  packFormatHoa->set(AudioPackFormatName("MyNewPackFormatHoa"));
  packFormatHoa->set(Importance(5));
  packFormatHoa->set(AbsoluteDistance(4.5));

  REQUIRE(packFormatHoa->has<AudioPackFormatId>());
  REQUIRE(packFormatHoa->has<AudioPackFormatName>());
  REQUIRE(packFormatHoa->has<TypeDescriptor>());
  REQUIRE(packFormatHoa->has<Importance>());
  REQUIRE(packFormatHoa->has<AbsoluteDistance>());

  REQUIRE(packFormatHoa->get<AudioPackFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::HOA);
  REQUIRE(
      packFormatHoa->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
      1u);
  REQUIRE(packFormatHoa->get<AudioPackFormatName>() == "MyNewPackFormatHoa");
  REQUIRE(packFormatHoa->get<TypeDescriptor>() == TypeDefinition::HOA);
  REQUIRE(packFormatHoa->get<Importance>() == 5);
  REQUIRE(packFormatHoa->get<AbsoluteDistance>() == 4.5);

  packFormatHoa->unset<Importance>();
  packFormatHoa->unset<AbsoluteDistance>();

  REQUIRE(!packFormatHoa->has<Importance>());
  REQUIRE(!packFormatHoa->has<AbsoluteDistance>());
}

TEST_CASE("audio_pack_format_hoa_with_all_extra_param") {
    using namespace adm;

    auto packFormatHoa =
        AudioPackFormatHoa::create(AudioPackFormatName("MyPackFormatHoa"));
    packFormatHoa->set(AudioPackFormatId(TypeDefinition::HOA,
                                         AudioPackFormatIdValue(1)));
    packFormatHoa->set(AudioPackFormatName("MyNewPackFormatHoa"));
    packFormatHoa->set(Normalization("N3D"));
    packFormatHoa->set(ScreenRef(true));
    packFormatHoa->set(NfcRefDist(2));

    REQUIRE(packFormatHoa->has<AudioPackFormatId>());
    REQUIRE(packFormatHoa->has<AudioPackFormatName>());
    REQUIRE(packFormatHoa->has<TypeDescriptor>());
    REQUIRE(packFormatHoa->has<Normalization>());
    REQUIRE(packFormatHoa->has<ScreenRef>());
    REQUIRE(packFormatHoa->has<NfcRefDist>());

    REQUIRE(packFormatHoa->get<AudioPackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::HOA);
    REQUIRE(
        packFormatHoa->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
        1u);
    REQUIRE(packFormatHoa->get<AudioPackFormatName>() == "MyNewPackFormatHoa");
    REQUIRE(packFormatHoa->get<TypeDescriptor>() ==
            TypeDefinition::HOA);
    REQUIRE(packFormatHoa->get<Normalization>() == "N3D");
    REQUIRE(packFormatHoa->get<ScreenRef>() == true);
    REQUIRE(packFormatHoa->get<NfcRefDist>() == 2);

    REQUIRE(!packFormatHoa->isDefault<Normalization>());
    REQUIRE(!packFormatHoa->isDefault<ScreenRef>());
    REQUIRE(!packFormatHoa->isDefault<NfcRefDist>());

    packFormatHoa->unset<Normalization>();
    packFormatHoa->unset<ScreenRef>();
    packFormatHoa->unset<NfcRefDist>();

    REQUIRE(packFormatHoa->isDefault<Normalization>());
    REQUIRE(packFormatHoa->isDefault<ScreenRef>());
    REQUIRE(packFormatHoa->isDefault<NfcRefDist>());

    REQUIRE(packFormatHoa->get<Normalization>() == "SN3D");
    REQUIRE(packFormatHoa->get<ScreenRef>() == false);
    REQUIRE(packFormatHoa->get<NfcRefDist>() == 0);

}

TEST_CASE("audio_pack_format_hoa_with_extra_param_in_constructor") {
    using namespace adm;

    auto packFormatHoa = AudioPackFormatHoa::create(
        AudioPackFormatName("MyPackFormatHoa"),
        AudioPackFormatId(AudioPackFormatIdValue(1), TypeDescriptor(4)),
        Normalization("FuMa"), ScreenRef(true), NfcRefDist(2));

    REQUIRE(packFormatHoa->has<AudioPackFormatId>());
    REQUIRE(packFormatHoa->has<AudioPackFormatName>());
    REQUIRE(packFormatHoa->has<TypeDescriptor>());
    REQUIRE(packFormatHoa->has<Normalization>());
    REQUIRE(packFormatHoa->has<ScreenRef>());
    REQUIRE(packFormatHoa->has<NfcRefDist>());

    REQUIRE(packFormatHoa->get<AudioPackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::HOA);
    REQUIRE(
        packFormatHoa->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
        1u);
    REQUIRE(packFormatHoa->get<AudioPackFormatName>() == "MyPackFormatHoa");
    REQUIRE(packFormatHoa->get<TypeDescriptor>() ==
            TypeDefinition::HOA);
    REQUIRE(packFormatHoa->get<Normalization>() == "FuMa");
    REQUIRE(packFormatHoa->get<ScreenRef>() == true);
    REQUIRE(packFormatHoa->get<NfcRefDist>() == 2);
}
