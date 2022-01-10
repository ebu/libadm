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
  SECTION("HeadphoneVirtualise") {
    HeadphoneVirtualise defaultHeadphoneVirtualise{};
    HeadphoneVirtualise customHeadphoneVirtualise{Bypass{true}};
    check_defaulted_param<HeadphoneVirtualise>(
        blockFormat, hasDefaultOf(defaultHeadphoneVirtualise),
        canBeSetTo(customHeadphoneVirtualise));
  }

  SECTION("headLocked") {
    check_defaulted_param<HeadLocked>(blockFormat,
                                      hasDefaultOf(HeadLocked{false}),
                                      canBeSetTo(HeadLocked{true}));
  }
}

TEMPLATE_TEST_CASE("gain parameter", "[parameter][template]",
                   adm::AudioBlockFormatDirectSpeakers,
                   adm::AudioBlockFormatMatrix, adm::AudioBlockFormatObjects,
                   adm::AudioBlockFormatHoa, adm::AudioBlockFormatBinaural) {
  auto blockFormat = BlockInitialiser<TestType>::create();
  check_defaulted_param<Gain>(blockFormat, hasDefaultOf(Gain::fromLinear(1.0f)),
                              canBeSetTo(Gain::fromLinear(2.5f)));
}

TEMPLATE_TEST_CASE("importance parameter", "[parameter][template]",
                   adm::AudioBlockFormatDirectSpeakers,
                   adm::AudioBlockFormatMatrix, adm::AudioBlockFormatObjects,
                   adm::AudioBlockFormatHoa, adm::AudioBlockFormatBinaural) {
  auto blockFormat = BlockInitialiser<TestType>::create();
  check_defaulted_param<Importance>(blockFormat, hasDefaultOf(10),
                                    canBeSetTo(1));
}
