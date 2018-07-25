#include <catch2/catch.hpp>
#include <typeinfo>
#include "adm/document.hpp"
#include "adm/elements.hpp"

std::shared_ptr<adm::AudioProgramme> dispatchCreate(const std::string& name,
                                                    adm::AudioProgramme::tag) {
  return adm::AudioProgramme::create(adm::AudioProgrammeName(name));
}
std::shared_ptr<adm::AudioContent> dispatchCreate(const std::string& name,
                                                  adm::AudioContent::tag) {
  return adm::AudioContent::create(adm::AudioContentName(name));
}
std::shared_ptr<adm::AudioObject> dispatchCreate(const std::string& name,
                                                 adm::AudioObject::tag) {
  return adm::AudioObject::create(adm::AudioObjectName(name));
}
std::shared_ptr<adm::AudioPackFormat> dispatchCreate(
    const std::string& name, adm::AudioPackFormat::tag) {
  return adm::AudioPackFormat::create(adm::AudioPackFormatName(name),
                                      adm::TypeDefinition::DIRECT_SPEAKERS);
}
std::shared_ptr<adm::AudioChannelFormat> dispatchCreate(
    const std::string& name, adm::AudioChannelFormat::tag) {
  return adm::AudioChannelFormat::create(adm::AudioChannelFormatName(name),
                                         adm::TypeDefinition::DIRECT_SPEAKERS);
}

std::shared_ptr<adm::AudioTrackUid> dispatchCreate(const std::string& /*name*/,
                                                   adm::AudioTrackUid::tag) {
  return adm::AudioTrackUid::create();
}

std::shared_ptr<adm::AudioTrackFormat> dispatchCreate(
    const std::string& name, adm::AudioTrackFormat::tag) {
  return adm::AudioTrackFormat::create(adm::AudioTrackFormatName(name),
                                       adm::FormatDefinition::PCM);
}

std::shared_ptr<adm::AudioStreamFormat> dispatchCreate(
    const std::string& name, adm::AudioStreamFormat::tag) {
  return adm::AudioStreamFormat::create(adm::AudioStreamFormatName(name),
                                        adm::FormatDefinition::PCM);
}

template <typename T>
std::shared_ptr<T> quickCreate(const std::string& name) {
  using Tag = typename T::tag;
  return dispatchCreate(name, Tag());
}

template <typename Referent, typename Reference>
void addReference(Referent referent, Reference reference) {
  referent->addReference(reference);
}

void addReference(std::shared_ptr<adm::AudioTrackUid> referent,
                  std::shared_ptr<adm::AudioTrackFormat> reference) {
  referent->setReference(reference);
}

void addReference(std::shared_ptr<adm::AudioTrackUid> referent,
                  std::shared_ptr<adm::AudioPackFormat> reference) {
  referent->setReference(reference);
}

void addReference(std::shared_ptr<adm::AudioTrackFormat> referent,
                  std::shared_ptr<adm::AudioStreamFormat> reference) {
  referent->setReference(reference);
}

void addReference(std::shared_ptr<adm::AudioStreamFormat> referent,
                  std::shared_ptr<adm::AudioTrackFormat> reference) {
  referent->addReference(std::weak_ptr<adm::AudioTrackFormat>(reference));
}

void addReference(std::shared_ptr<adm::AudioStreamFormat> referent,
                  std::shared_ptr<adm::AudioChannelFormat> reference) {
  referent->setReference(reference);
}

void addReference(std::shared_ptr<adm::AudioStreamFormat> referent,
                  std::shared_ptr<adm::AudioPackFormat> reference) {
  referent->setReference(reference);
}

template <typename Referent, typename Reference>
inline void verifyByParent() {
  const std::string referentStr = typeid(Referent).name();
  const std::string referenceStr = typeid(Reference).name();
  SECTION("verify:" + referentStr + "->" + referenceStr) {
    auto admDocument = adm::Document::create();
    auto referent = quickCreate<Referent>("referent");
    auto reference = quickCreate<Reference>("reference");
    admDocument->add(referent);
    addReference(referent, reference);
    // the reference must now be a part of the document as well
    REQUIRE(referent->getParent().lock() == admDocument);
    REQUIRE(reference->getParent().lock() == admDocument);
  }
}

TEST_CASE("by_parent") {
  verifyByParent<adm::AudioProgramme, adm::AudioContent>();
  verifyByParent<adm::AudioProgramme, adm::AudioContent>();
  verifyByParent<adm::AudioContent, adm::AudioObject>();
  verifyByParent<adm::AudioObject, adm::AudioObject>();
  verifyByParent<adm::AudioObject, adm::AudioPackFormat>();
  verifyByParent<adm::AudioObject, adm::AudioTrackUid>();
  verifyByParent<adm::AudioPackFormat, adm::AudioChannelFormat>();
  verifyByParent<adm::AudioPackFormat, adm::AudioPackFormat>();
  verifyByParent<adm::AudioTrackUid, adm::AudioTrackFormat>();
  verifyByParent<adm::AudioTrackUid, adm::AudioPackFormat>();
  verifyByParent<adm::AudioTrackFormat, adm::AudioStreamFormat>();
  verifyByParent<adm::AudioStreamFormat, adm::AudioTrackFormat>();
  verifyByParent<adm::AudioStreamFormat, adm::AudioChannelFormat>();
  verifyByParent<adm::AudioStreamFormat, adm::AudioPackFormat>();
}

template <typename Referent, typename Reference>
inline void verifyByChild() {
  const std::string referentStr = typeid(Referent).name();
  const std::string referenceStr = typeid(Reference).name();
  SECTION("verify:" + referentStr + "->" + referenceStr) {
    using namespace adm;
    auto admDocument = Document::create();
    auto referent = quickCreate<Referent>("referent");
    auto reference = quickCreate<Reference>("reference");
    admDocument->add(reference);
    addReference(referent, reference);

    // the reference must now be a part of the document as well
    REQUIRE(referent->getParent().lock() == admDocument);
    REQUIRE(reference->getParent().lock() == admDocument);
  }
}

TEST_CASE("by_child") {
  verifyByChild<adm::AudioProgramme, adm::AudioContent>();
  verifyByChild<adm::AudioProgramme, adm::AudioContent>();
  verifyByChild<adm::AudioContent, adm::AudioObject>();
  verifyByChild<adm::AudioObject, adm::AudioObject>();
  verifyByChild<adm::AudioObject, adm::AudioPackFormat>();
  verifyByChild<adm::AudioObject, adm::AudioTrackUid>();
  verifyByChild<adm::AudioPackFormat, adm::AudioChannelFormat>();
  verifyByChild<adm::AudioPackFormat, adm::AudioPackFormat>();
  verifyByChild<adm::AudioTrackUid, adm::AudioTrackFormat>();
  verifyByChild<adm::AudioTrackUid, adm::AudioPackFormat>();
  verifyByChild<adm::AudioTrackFormat, adm::AudioStreamFormat>();
  verifyByChild<adm::AudioStreamFormat, adm::AudioTrackFormat>();
  verifyByChild<adm::AudioStreamFormat, adm::AudioChannelFormat>();
  verifyByChild<adm::AudioStreamFormat, adm::AudioPackFormat>();
}

template <typename Referent, typename Reference>
inline void verifyWithoutParent() {
  const std::string referentStr = typeid(Referent).name();
  const std::string referenceStr = typeid(Reference).name();
  SECTION("verify:" + referentStr + "->" + referenceStr) {
    using namespace adm;
    auto admDocument = Document::create();
    auto referent = quickCreate<Referent>("referent");
    auto reference = quickCreate<Reference>("reference");

    addReference(referent, reference);
    REQUIRE(referent->getParent().lock() == nullptr);
    REQUIRE(reference->getParent().lock() == nullptr);
  }
}

TEST_CASE("without_parent") {
  verifyWithoutParent<adm::AudioProgramme, adm::AudioContent>();
  verifyWithoutParent<adm::AudioProgramme, adm::AudioContent>();
  verifyWithoutParent<adm::AudioContent, adm::AudioObject>();
  verifyWithoutParent<adm::AudioObject, adm::AudioObject>();
  verifyWithoutParent<adm::AudioObject, adm::AudioPackFormat>();
  verifyWithoutParent<adm::AudioObject, adm::AudioTrackUid>();
  verifyWithoutParent<adm::AudioPackFormat, adm::AudioChannelFormat>();
  verifyWithoutParent<adm::AudioPackFormat, adm::AudioPackFormat>();
  verifyWithoutParent<adm::AudioTrackUid, adm::AudioTrackFormat>();
  verifyWithoutParent<adm::AudioTrackUid, adm::AudioPackFormat>();
  verifyWithoutParent<adm::AudioTrackFormat, adm::AudioStreamFormat>();
  verifyWithoutParent<adm::AudioStreamFormat, adm::AudioTrackFormat>();
  verifyWithoutParent<adm::AudioStreamFormat, adm::AudioChannelFormat>();
  verifyWithoutParent<adm::AudioStreamFormat, adm::AudioPackFormat>();
}

template <typename Referent, typename Reference>
inline void verifyDifferentParent() {
  const std::string referentStr = typeid(Referent).name();
  const std::string referenceStr = typeid(Reference).name();
  SECTION("verify:" + referentStr + "->" + referenceStr) {
    using namespace adm;
    auto admDocument1 = Document::create();
    auto admDocument2 = Document::create();
    auto referent = quickCreate<Referent>("referent");
    auto reference = quickCreate<Reference>("reference");

    // currently, we have no means to check if there's no parent,
    // but if this compiles there are not many other options
    admDocument1->add(referent);
    admDocument2->add(reference);
    REQUIRE_THROWS_AS(addReference(referent, reference), std::runtime_error);
  }
}

TEST_CASE("different_parent") {
  verifyDifferentParent<adm::AudioProgramme, adm::AudioContent>();
  verifyDifferentParent<adm::AudioProgramme, adm::AudioContent>();
  verifyDifferentParent<adm::AudioContent, adm::AudioObject>();
  verifyDifferentParent<adm::AudioObject, adm::AudioObject>();
  verifyDifferentParent<adm::AudioObject, adm::AudioPackFormat>();
  verifyDifferentParent<adm::AudioObject, adm::AudioTrackUid>();
  verifyDifferentParent<adm::AudioPackFormat, adm::AudioChannelFormat>();
  verifyDifferentParent<adm::AudioPackFormat, adm::AudioPackFormat>();
  verifyDifferentParent<adm::AudioTrackUid, adm::AudioTrackFormat>();
  verifyDifferentParent<adm::AudioTrackUid, adm::AudioPackFormat>();
  verifyDifferentParent<adm::AudioTrackFormat, adm::AudioStreamFormat>();
  verifyDifferentParent<adm::AudioStreamFormat, adm::AudioTrackFormat>();
  verifyDifferentParent<adm::AudioStreamFormat, adm::AudioChannelFormat>();
  verifyDifferentParent<adm::AudioStreamFormat, adm::AudioPackFormat>();
}
