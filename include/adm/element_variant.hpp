#pragma once

#include "adm/elements.hpp"
#include <boost/variant.hpp>
#include <memory>

namespace adm {
  using ElementVariant = boost::variant<
      std::shared_ptr<adm::AudioProgramme>, std::shared_ptr<adm::AudioContent>,
      std::shared_ptr<adm::AudioObject>, std::shared_ptr<adm::AudioPackFormat>,
      std::shared_ptr<adm::AudioChannelFormat>,
      std::shared_ptr<adm::AudioStreamFormat>,
      std::shared_ptr<adm::AudioTrackFormat>,
      std::shared_ptr<adm::AudioTrackUid>>;

  using ElementConstVariant =
      boost::variant<std::shared_ptr<const AudioProgramme>,
                     std::shared_ptr<const AudioContent>,
                     std::shared_ptr<const AudioObject>,
                     std::shared_ptr<const AudioTrackUid>,
                     std::shared_ptr<const AudioPackFormat>,
                     std::shared_ptr<const AudioChannelFormat>,
                     std::shared_ptr<const AudioStreamFormat>,
                     std::shared_ptr<const AudioTrackFormat>>;

  template <typename VariantType, typename Variant>
  struct IsVariantType {
    bool operator()(const Variant& v) const {
      return v.type() == typeid(VariantType);
    }
  };

  template <typename VariantType, typename Variant>
  bool isVariantType(const Variant& v) {
    return v.type() == typeid(VariantType);
  }

  template <typename Element>
  std::shared_ptr<Element> getElementOr(
      const ElementVariant& variant,
      std::shared_ptr<Element> elseValue = nullptr) {
    if (isVariantType<std::shared_ptr<Element>>(variant)) {
      return boost::get<std::shared_ptr<Element>>(variant);
    } else {
      return elseValue;
    }
  }

  template <typename Element>
  std::shared_ptr<const Element> getElementOr(
      const ElementConstVariant& variant,
      std::shared_ptr<const Element> elseValue = nullptr) {
    if (isVariantType<std::shared_ptr<const Element>>(variant)) {
      return boost::get<std::shared_ptr<const Element>>(variant);
    } else {
      return elseValue;
    }
  }
  using ElementIdVariant =
      boost::variant<adm::AudioProgrammeId, adm::AudioContentId,
                     adm::AudioObjectId, adm::AudioChannelFormatId,
                     adm::AudioPackFormatId, adm::AudioTrackUidId,
                     adm::AudioTrackFormatId, adm::AudioStreamFormatId>;

}  // namespace adm
