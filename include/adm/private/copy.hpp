#pragma once

#include <algorithm>
#include <boost/variant.hpp>
#include <unordered_map>
#include <memory>
#include "adm/elements.hpp"
#include "adm/element_variant.hpp"

namespace adm {

  /**
   * @brief Per-element-kind shared_ptr mapping from a source Document's
   * elements to their copies. Populated by `copyAllElements` and consumed
   * by reference-resolution helpers such as `resolveReferences` and
   * `copyAuxiliary`.
   */
  struct ElementMapping {
    // clang-format off
    std::unordered_map<std::shared_ptr<const AudioProgramme>, std::shared_ptr<AudioProgramme>> audioProgramme;
    std::unordered_map<std::shared_ptr<const AudioContent>, std::shared_ptr<AudioContent>> audioContent;
    std::unordered_map<std::shared_ptr<const AudioObject>, std::shared_ptr<AudioObject>> audioObject;
    std::unordered_map<std::shared_ptr<const AudioPackFormat>, std::shared_ptr<AudioPackFormat>> audioPackFormat;
    std::unordered_map<std::shared_ptr<const AudioChannelFormat>, std::shared_ptr<AudioChannelFormat>> audioChannelFormat;
    std::unordered_map<std::shared_ptr<const AudioStreamFormat>, std::shared_ptr<AudioStreamFormat>> audioStreamFormat;
    std::unordered_map<std::shared_ptr<const AudioTrackFormat>, std::shared_ptr<AudioTrackFormat>> audioTrackFormat;
    std::unordered_map<std::shared_ptr<const AudioTrackUid>, std::shared_ptr<AudioTrackUid>> audioTrackUid;
    // clang-format on
  };

  std::vector<ElementVariant> copyAllElements(
      std::shared_ptr<const Document> document);

  /**
   * @brief Like `copyAllElements`, but also reports the source-to-copy
   * mapping for every element kind, so callers can translate references
   * stored in document-level parameters (e.g. tagList).
   */
  std::vector<ElementVariant> copyAllElements(
      std::shared_ptr<const Document> document, ElementMapping& mapping);

  /**
   * @brief Copy document-level auxiliary parameters (ProfileList, TagList)
   * from `src` to `dest`, translating any element references in the
   * TagList through `mapping`.
   *
   * Must be called *after* the copied elements have been added to `dest`,
   * so that `Document::set(TagList)` can adopt them via paternity checks.
   */
  void copyAuxiliary(std::shared_ptr<const Document> src,
                     std::shared_ptr<Document> dest,
                     ElementMapping const& mapping);

  template <typename C>
  class AddTo : public boost::static_visitor<> {
   public:
    explicit AddTo(C& target) : target_(target) {}

    template <typename T>
    void operator()(const T& element) const {
      target_->add(element);
    }

   private:
    C& target_;
  };

  namespace visitor {
    template <typename C>
    AddTo<C> addTo(C& target) {
      return AddTo<C>(target);
    }
  }  // namespace visitor

  template <typename Target>
  void addElements(std::vector<ElementVariant> elements,
                   std::shared_ptr<Target> target) {
    for (auto& element : elements) {
      boost::apply_visitor(visitor::addTo(target), element);
    }
  }

  template <typename ElementSrc, typename ElementDest>
  void resolveReferences(
      const std::shared_ptr<const ElementSrc>& element,
      const std::unordered_map<std::shared_ptr<const ElementSrc>,
                               std::shared_ptr<ElementSrc>>& mappingSrc,
      const std::unordered_map<std::shared_ptr<const ElementDest>,
                               std::shared_ptr<ElementDest>>& mappingDest) {
    for (const auto& reference :
         element->template getReferences<ElementDest>()) {
      mappingSrc.at(element)->addReference(mappingDest.at(reference));
    }
  }

  inline void resolveReferences(
      const std::shared_ptr<const AudioStreamFormat>& element,
      const std::unordered_map<std::shared_ptr<const AudioStreamFormat>,
                               std::shared_ptr<AudioStreamFormat>>& mappingSrc,
      const std::unordered_map<std::shared_ptr<const AudioTrackFormat>,
                               std::shared_ptr<AudioTrackFormat>>&
          mappingDest) {
    for (const auto& weakReference : element->getAudioTrackFormatReferences()) {
      auto reference = weakReference.lock();
      if (reference) {
        mappingSrc.at(element)->addReference(
            std::weak_ptr<AudioTrackFormat>(mappingDest.at(reference)));
      }
    }
  }

  template <typename ElementSrc, typename ElementDest>
  void resolveReference(
      const std::shared_ptr<const ElementSrc>& element,
      const std::unordered_map<std::shared_ptr<const ElementSrc>,
                               std::shared_ptr<ElementSrc>>& mappingSrc,
      const std::unordered_map<std::shared_ptr<const ElementDest>,
                               std::shared_ptr<ElementDest>>& mappingDest) {
    if (auto reference = element->template getReference<ElementDest>()) {
      mappingSrc.at(element)->setReference(mappingDest.at(reference));
    }
  }

  template <typename ElementSrc, typename ElementDest>
  void resolveComplementaries(
      const std::shared_ptr<const ElementSrc>& element,
      const std::unordered_map<std::shared_ptr<const ElementSrc>,
                               std::shared_ptr<ElementSrc>>& mappingSrc,
      const std::unordered_map<std::shared_ptr<const ElementDest>,
                               std::shared_ptr<ElementDest>>& mappingDest) {
    for (const auto& reference : element->getComplementaryObjects()) {
      mappingSrc.at(element)->addComplementary(mappingDest.at(reference));
    }
  }

  /**
   * @brief Unary predicate which compares the given Parameter
   */
  template <typename Element, typename Parameter>
  class ParameterEqualTo {
   public:
    explicit ParameterEqualTo(const Element& element)
        : parameter_(element.template get<Parameter>()) {}

    inline bool operator()(const Element& other) const {
      return parameter_ == other.template get<Parameter>();
    }

   private:
    Parameter parameter_;
  };

  /**
   * @brief Copy AudioBlockFormats from one AudioChannelFormat to another
   *
   * This function will only copy audioBlockFormats which are not already
   * part of the destination audioChannelFormat. The check whether they are
   * already there is done using the id.
   */
  template <typename AudioBlockFormat>
  void copyAudioBlockFormats(
      const std::shared_ptr<const AudioChannelFormat>& src,
      const std::shared_ptr<AudioChannelFormat>& dest) {
    auto blockFormatsSrc = src->template getElements<AudioBlockFormat>();
    auto blockFormatsDest = dest->template getElements<AudioBlockFormat>();
    for (const auto& blockFormatSrc : blockFormatsSrc) {
      auto foundBlockFormat =
          std::find_if(blockFormatsDest.begin(), blockFormatsDest.end(),
                       ParameterEqualTo<AudioBlockFormat, AudioBlockFormatId>(
                           blockFormatSrc));
      if (foundBlockFormat == blockFormatsDest.end()) {
        dest->add(blockFormatSrc);
      }
    }
  }

}  // namespace adm
