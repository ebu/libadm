#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/common_definitions.hpp"

#define BOOST_TEST_MODULE CreateAdm
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(basic_document) {
  using namespace adm;

  // create a basic document
  { auto commonDefinitions = getCommonDefinitions(); }
}
