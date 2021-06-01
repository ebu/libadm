#include "helper/parameter_checks.hpp"
#include <adm/elements.hpp>
namespace adm_test {
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
}  // namespace adm_test

TEMPLATE_TEST_CASE("Common audioBlockFormat parameter checks",
                   "[parameter][template]", adm::AudioBlockFormatObjects,
                   adm::AudioBlockFormatDirectSpeakers,
                   adm::AudioBlockFormatMatrix, adm::AudioBlockFormatHoa,
                   adm::AudioBlockFormatBinaural) {
  using namespace adm_test;
  using namespace adm;
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
