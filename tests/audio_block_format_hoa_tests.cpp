#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_hoa.hpp"

TEST_CASE("audio_block_format_hoa") {
  using namespace adm;
  {
    AudioBlockFormatHoa blockFormat{Order(), Degree()};

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<Order>() == true);
    REQUIRE(blockFormat.has<Degree>() == true);
    REQUIRE(blockFormat.has<NfcRefDist>() == false);
    REQUIRE(blockFormat.has<ScreenRef>() == false);
    REQUIRE(blockFormat.has<Normalization>() == false);
    REQUIRE(blockFormat.has<Equation>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.isDefault<NfcRefDist>() == true);
    REQUIRE(blockFormat.isDefault<ScreenRef>() == true);
    REQUIRE(blockFormat.isDefault<Normalization>() == true);
    // Equation has no default value - BS.2076-2 Section 5.4.3.4

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(1)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(Order(1));
    blockFormat.set(Degree(-1));
    blockFormat.set(Degree(-2));
    blockFormat.set(NfcRefDist(2));
    blockFormat.set(ScreenRef(1));
    blockFormat.set(Normalization("SN3D"));
    blockFormat.set(Equation("cos(A)*sin(E)"));

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == true);
    REQUIRE(blockFormat.has<Order>() == true);
    REQUIRE(blockFormat.has<Degree>() == true);
    REQUIRE(blockFormat.has<NfcRefDist>() == true);
    REQUIRE(blockFormat.has<ScreenRef>() == true);
    REQUIRE(blockFormat.has<Normalization>() == true);
    REQUIRE(blockFormat.has<Equation>() == true);

    REQUIRE(blockFormat.isDefault<Rtime>() == false);
    REQUIRE(blockFormat.isDefault<Duration>() == false);
    REQUIRE(blockFormat.isDefault<NfcRefDist>() == false);
    REQUIRE(blockFormat.isDefault<ScreenRef>() == false);
    REQUIRE(blockFormat.isDefault<Normalization>() == false);
    // Equation has no default value - BS.2076-2 Section 5.4.3.4

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(1));
    REQUIRE(blockFormat.get<Duration>().get() == std::chrono::seconds(10));
    REQUIRE(blockFormat.get<Order>() == 1);
    REQUIRE(blockFormat.get<Degree>() == -2);
    REQUIRE(blockFormat.get<NfcRefDist>() == 2);
    REQUIRE(blockFormat.get<ScreenRef>() == 1);
    REQUIRE(blockFormat.get<Normalization>() == "SN3D");
    REQUIRE(blockFormat.get<Equation>() == "cos(A)*sin(E)");

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<NfcRefDist>();
    blockFormat.unset<ScreenRef>();
    blockFormat.unset<Normalization>();
    blockFormat.unset<Equation>();

    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<Order>() == true);
    REQUIRE(blockFormat.has<Degree>() == true);
    REQUIRE(blockFormat.has<NfcRefDist>() == false);
    REQUIRE(blockFormat.has<ScreenRef>() == false);
    REQUIRE(blockFormat.has<Normalization>() == false);
    REQUIRE(blockFormat.has<Equation>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
  }
}
