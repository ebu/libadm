#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/time.hpp"
#include "adm/serialized/frame_format.hpp"
#include "adm/serialized/frame_header.hpp"

#define BOOST_TEST_MODULE FrameFormat
#include <boost/test/included/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(frame_format_tests) {
  using namespace adm;
  // Attributes / Elements
  {
    FrameFormat frameFormat(FrameStart(std::chrono::seconds(10)),
                            FrameDuration(std::chrono::seconds(20)),
                            FrameType("header"), TimeReference("total"));

    BOOST_TEST(frameFormat.has<FrameFormatId>() == true);
    BOOST_TEST(frameFormat.has<FrameStart>() == true);
    BOOST_TEST(frameFormat.has<FrameDuration>() == true);
    BOOST_TEST(frameFormat.has<TimeReference>() == true);
    BOOST_TEST(frameFormat.has<FrameType>() == true);
    BOOST_TEST(frameFormat.has<CountToFull>() == false);
    BOOST_TEST(frameFormat.has<NumSubFrame>() == false);
    BOOST_TEST(frameFormat.has<FrameSkip>() == false);
    BOOST_TEST(frameFormat.has<FrameShift>() == false);

    frameFormat.set(CountToFull(1));
    frameFormat.set(NumSubFrame(1));
    frameFormat.set(FrameSkip(1));
    frameFormat.set(FrameShift(1));

    BOOST_TEST(frameFormat.has<CountToFull>() == true);
    BOOST_TEST(frameFormat.has<NumSubFrame>() == true);
    BOOST_TEST(frameFormat.has<FrameSkip>() == true);
    BOOST_TEST(frameFormat.has<FrameShift>() == true);

    BOOST_TEST(frameFormat.get<FrameFormatId>().get<FrameFormatIdValue>() ==
               0u);
    BOOST_TEST(frameFormat.get<TimeReference>() == "total");
    BOOST_TEST(frameFormat.get<FrameType>() == "header");
    BOOST_CHECK(frameFormat.get<FrameStart>() == std::chrono::seconds(10));
    BOOST_CHECK(frameFormat.get<FrameDuration>() == std::chrono::seconds(20));
    BOOST_TEST(frameFormat.get<CountToFull>().get() == 1u);
    BOOST_TEST(frameFormat.get<NumSubFrame>().get() == 1u);
    BOOST_TEST(frameFormat.get<FrameSkip>().get() == 1u);
    BOOST_TEST(frameFormat.get<FrameShift>().get() == 1u);

    frameFormat.unset<CountToFull>();
    frameFormat.unset<NumSubFrame>();
    frameFormat.unset<FrameSkip>();
    frameFormat.unset<FrameShift>();

    BOOST_TEST(frameFormat.has<CountToFull>() == false);
    BOOST_TEST(frameFormat.has<NumSubFrame>() == false);
    BOOST_TEST(frameFormat.has<FrameSkip>() == false);
    BOOST_TEST(frameFormat.has<FrameShift>() == false);
  }
}
