#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"

#include "adm/serialized/frame_header.hpp"

#define BOOST_TEST_MODULE FrameHeader
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(frame_header) {
  using namespace adm;

  // create a basic document
  {
    FrameHeader frameHeader(FrameStart(std::chrono::nanoseconds(0)),
                            FrameDuration(std::chrono::nanoseconds(1)),
                            FrameType("full"));
  }
}
