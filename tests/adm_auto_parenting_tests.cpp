#include <catch2/catch.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <typeinfo>
#include "adm/document.hpp"
#include "adm/elements.hpp"

#include <algorithm>

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

template <typename T1, typename T2>
struct ReferenceDefinition {
  using referent = T1;
  using reference = T2;
};

using PossibleReferences = boost::mpl::list<
    ReferenceDefinition<adm::AudioProgramme, adm::AudioContent>,
    ReferenceDefinition<adm::AudioContent, adm::AudioObject>,
    ReferenceDefinition<adm::AudioObject, adm::AudioObject>,
    ReferenceDefinition<adm::AudioObject, adm::AudioPackFormat>,
    ReferenceDefinition<adm::AudioObject, adm::AudioTrackUid>,
    ReferenceDefinition<adm::AudioPackFormat, adm::AudioChannelFormat>,
    ReferenceDefinition<adm::AudioPackFormat, adm::AudioPackFormat>,
    ReferenceDefinition<adm::AudioTrackUid, adm::AudioTrackFormat>,
    ReferenceDefinition<adm::AudioTrackUid, adm::AudioPackFormat>,
    ReferenceDefinition<adm::AudioTrackFormat, adm::AudioStreamFormat>,
    ReferenceDefinition<adm::AudioStreamFormat, adm::AudioTrackFormat>,
    ReferenceDefinition<adm::AudioStreamFormat, adm::AudioChannelFormat>,
    ReferenceDefinition<adm::AudioStreamFormat, adm::AudioPackFormat>>;

struct verifyByParent {
  template <typename T>
  void operator()(T) {
    using Referent = typename T::referent;
    using Reference = typename T::reference;
    const std::string referentStr = typeid(Referent).name();
    const std::string referenceStr = typeid(Reference).name();
    SECTION("verify:" + referentStr + "->" + referenceStr) {
      using namespace adm;
      auto admDocument = Document::create();
      auto referent = quickCreate<Referent>("referent");
      auto reference = quickCreate<Reference>("reference");
      admDocument->add(referent);
      addReference(referent, reference);

      // the reference must now be a part of the document as well
      REQUIRE(referent->getParent().lock() == admDocument);
      REQUIRE(reference->getParent().lock() == admDocument);
    }
  }
};

TEST_CASE("by_parent") {
  boost::mpl::for_each<PossibleReferences>(verifyByParent());
}

struct verifyByChild {
  template <typename T>
  void operator()(T) {
    using Referent = typename T::referent;
    using Reference = typename T::reference;
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
};

TEST_CASE("by_child") {
  boost::mpl::for_each<PossibleReferences>(verifyByChild());
}

struct verifyWithoutParent {
  template <typename T>
  void operator()(T) {
    using Referent = typename T::referent;
    using Reference = typename T::reference;
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
};

TEST_CASE("without_parent") {
  boost::mpl::for_each<PossibleReferences>(verifyWithoutParent());
}

struct verifyDifferentParent {
  template <typename T>
  void operator()(T) {
    using Referent = typename T::referent;
    using Reference = typename T::reference;
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
};

TEST_CASE("different_parent") {
  boost::mpl::for_each<PossibleReferences>(verifyDifferentParent());
}
