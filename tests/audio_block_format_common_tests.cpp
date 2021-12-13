#include "helper/parameter_checks.hpp"
#include <adm/elements.hpp>

using namespace adm_test;
using namespace adm;

template <typename T>
struct BlockInitialiser {
  static T create() { return T{}; }
};
template <>
struct BlockInitialiser<adm::AudioBlockFormatHoa> {
  static adm::AudioBlockFormatHoa create() {
    return adm::AudioBlockFormatHoa{adm::Order{}, adm::Degree{}};
  }
};
template <>
struct BlockInitialiser<adm::AudioBlockFormatObjects> {
  static adm::AudioBlockFormatObjects create() {
    return adm::AudioBlockFormatObjects{adm::SphericalPosition{}};
  }
};

TEMPLATE_TEST_CASE("headphone parameter checks", "[parameter][template]",
                   adm::AudioBlockFormatObjects,
                   adm::AudioBlockFormatDirectSpeakers,
                   adm::AudioBlockFormatHoa) {
  auto blockFormat = BlockInitialiser<TestType>::create();
  // TODO Gain, Importance, HeadLocked
  SECTION("HeadphoneVirtualise") {
    HeadphoneVirtualise defaultHeadphoneVirtualise{};
    HeadphoneVirtualise customHeadphoneVirtualise{Bypass{true}};
    check_defaulted_param<HeadphoneVirtualise>(
        blockFormat, hasDefaultOf(defaultHeadphoneVirtualise),
        canBeSetTo(customHeadphoneVirtualise));
  }
}
