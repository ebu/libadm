#include <catch2/catch.hpp>
#include <adm/document.hpp>
#include <adm/helper/visit.hpp>
#include <adm/utilities/object_creation.hpp>
//
// Created by Richard Bailey on 01/07/2022.
//

using namespace adm;

struct ElementCounts {
  std::size_t programmes{0};
  std::size_t contents{0};
  std::size_t objects{0};
  std::size_t tracks{0};
  std::size_t packs{0};
  std::size_t channels{0};
  std::size_t streams{0};
  std::size_t trackUids{0};
};

struct ElementCounter {
  // The ctors are only declared for the purposes of testing, a plain struct
  // would work just fine.
  ElementCounter() = default;
  ElementCounter(ElementCounter const& other) = delete;
  ElementCounter(ElementCounter && other) noexcept = default;
  void operator()(AudioProgramme const& programme) { ++counts.programmes; }
  void operator()(AudioContent const& content) { ++counts.contents; }
  void operator()(AudioObject const& object) { ++counts.objects; }
  void operator()(AudioTrackFormat const& track) { ++counts.tracks; }
  void operator()(AudioPackFormat const& pack) { ++counts.packs; }
  void operator()(AudioChannelFormat const& pack) { ++counts.channels; }
  void operator()(AudioStreamFormat const& stream) { ++counts.streams; }
  void operator()(AudioTrackUid const& trackUid) { ++counts.trackUids; }
  ElementCounts counts;
};

TEST_CASE("Visit document") {
  auto doc = Document::create();
  addSimpleObjectTo(doc, "Test");

  using DocElements = ElementList<AudioProgramme, AudioContent, AudioObject,
        AudioPackFormat, AudioChannelFormat, AudioTrackFormat,
        AudioStreamFormat, AudioTrackUid>;

  RangeVisitor<ElementCounter> counter;
  // That this works proves visitor can be passed as lvalue ref
  // If it couldn't the counts would all be 0
  visit(getElementRanges<DocElements>(*doc), counter);
  auto counts = counter.counts;
  REQUIRE(counts.programmes == 0);
  REQUIRE(counts.contents == 0);
  REQUIRE(counts.objects == 1);
  REQUIRE(counts.tracks == 1);
  REQUIRE(counts.packs == 1);
  REQUIRE(counts.channels == 1);
  REQUIRE(counts.streams == 1);
  REQUIRE(counts.trackUids == 1);

  // lvalue ref can't bind to temporary, copy ctor is deleted -
  // That this compiles proves visitor can be passed as rvalue ref
  visit(getElementRanges<DocElements>(*doc), RangeVisitor<ElementCounter>{});
}