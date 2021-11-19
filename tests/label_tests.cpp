#include <catch2/catch.hpp>
#include "adm/elements/label.hpp"
#include "helper/parameter_checks.hpp"

using namespace adm;
using namespace adm_test;

TEST_CASE("label") {
  Label label(LabelValue("label"));

  check_required_param<LabelValue>(label, hasDefaultOf("label"),
                                   canBeSetTo("foo"));
  check_optional_param<LabelLanguage>(label, canBeSetTo("EN"));
}
