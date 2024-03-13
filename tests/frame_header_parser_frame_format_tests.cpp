#include <catch2/catch.hpp>
#include <sstream>
#include <adm/serial.hpp>
#include <adm/parse.hpp>

namespace {
  constexpr const char* MINIMAL_HEADER =
      R"(<?xml version="1.0" encoding="utf-8"?>
<frame>
    <frameHeader>
        <frameFormat frameFormatID="FF_00000001" start="00:01:00.00000" duration="00:00:10.00000" type="full">
        </frameFormat>
        <transportTrackFormat transportID="TP_0001">
            <audioTrack trackID="1">
               <audioTrackUidRef>ATU_00000001</audioTrackUidRef>
            </audioTrack>
        </transportTrackFormat>
    </frameHeader>
</frame>
)";
}

TEST_CASE("Test minimal frame header parsing") {
  std::stringstream header_xml{MINIMAL_HEADER};
  CHECK_NOTHROW(adm::parseFrameHeader(header_xml));
}