#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_pack_format_hoa.hpp"
#include "helper/parameter_checks.hpp"

TEST_CASE("audio_pack_format_hoa") {
  using namespace adm;
  using namespace adm_test;
  auto packFormatHoa =
      AudioPackFormatHoa::create(AudioPackFormatName("MyPackFormatHoa"));

  check_required_param<AudioPackFormatName>(
      packFormatHoa, hasDefaultOf(AudioPackFormatName("MyPackFormatHoa")),
      canBeSetTo(AudioPackFormatName("MyPackFormatHoaTest")));

  // no default is specified for audioPackFormat for some reason
  check_optional_param<Importance>(packFormatHoa, canBeSetTo(Importance(5)));

  check_optional_param<AbsoluteDistance>(packFormatHoa,
                                         canBeSetTo(AbsoluteDistance(2.0)));

  check_defaulted_param<Normalization>(packFormatHoa,
                                       hasDefaultOf(Normalization("SN3D")),
                                       canBeSetTo(Normalization("N3D")));

  check_defaulted_param<NfcRefDist>(packFormatHoa,
                                    hasDefaultOf(NfcRefDist(1.0)),
                                    canBeSetTo(NfcRefDist(0.0)));

  check_defaulted_param<ScreenRef>(packFormatHoa, hasDefaultOf(ScreenRef(true)),
                                   canBeSetTo(ScreenRef(false)));
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
  REQUIRE(packFormatHoa->get<TypeDescriptor>() == TypeDefinition::HOA);
  REQUIRE(packFormatHoa->get<Normalization>() == "FuMa");
  REQUIRE(packFormatHoa->get<ScreenRef>() == true);
  REQUIRE(packFormatHoa->get<NfcRefDist>() == 2);
}
