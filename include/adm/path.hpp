#pragma once

#include "adm/elements_fwd.hpp"
#include "adm/element_variant.hpp"
#include "adm/frame.hpp"
#include <boost/functional/hash.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "adm/libadm_export.h"

namespace adm {

  /**
   * Describes path along ADM elements within an ADM document, i.e.
   * which references were followed to reach a certain element from a
   * given start point.
   *
   * This is the same purpose (and basic interface) as `adm::Route`,
   * but currently the implementation and constraints are different.
   *
   * Thus, the name `Path` has been chosen to distinguish between those
   * two.
   *
   * The main/essential difference is that the `Path` does not store
   * pointers to the ADM elements, as they might become invalid. Instead
   * the path stores the (coded) element ids, which are also used to check
   * if Paths are equal or not.
   *
   * The hash alone is not useful, as we need locate elements within the
   * path again (This also solves the problem of potential hash
   * colisisions).
   *
   * Hashes are still a usefull tool when working with a known set of
   * routes, as in this case has collosisions can be checked up-front.
   */
  class Path {
   public:
    typedef std::size_t hash_type;
    typedef std::vector<ElementIdVariant>::iterator iterator;
    typedef std::vector<ElementIdVariant>::const_iterator const_iterator;

    template <typename AdmId>
    void add(AdmId id) {
      elements_.push_back(id);
      boost::hash_combine(hash_, formatId(id));
    }

    template <typename Element>
    void add(std::shared_ptr<const Element> element) {
      using ElementId = typename Element::id_type;
      auto id = element->template get<ElementId>();
      add(id);
    }

    LIBADM_EXPORT void add(ElementConstVariant elementVariant);
    LIBADM_EXPORT void add(ElementIdVariant elementIdVariant);

    hash_type hash() const { return hash_; }

    const_iterator begin() const { return elements_.begin(); }
    const_iterator end() const { return elements_.end(); }

    ElementIdVariant front() const { return elements_.front(); }
    ElementIdVariant back() const { return elements_.back(); }

    std::size_t size() const { return elements_.size(); }

    template <typename AdmElementId>
    AdmElementId getFirstOf() const;
    template <typename AdmElementId>
    AdmElementId getLastOf() const;

    template <typename AdmElementId>
    bool hasIdType() const;

   private:
    friend bool operator==(const Path& lhs, const Path& rhs);
    friend bool operator<(const Path& lhs, const Path& rhs);
    std::vector<ElementIdVariant> elements_;
    hash_type hash_ = 0;
  };

  template <typename AdmElementId>
  AdmElementId Path::getFirstOf() const {
    auto foundFirst =
        std::find_if(elements_.begin(), elements_.end(),
                     IsVariantType<AdmElementId, ElementIdVariant>());

    if (foundFirst == elements_.end()) {
      throw std::logic_error("No element id of given type found");
    }
    return boost::get<AdmElementId>(*foundFirst);
  }

  template <typename AdmElementId>
  AdmElementId Path::getLastOf() const {
    auto foundLast =
        std::find_if(elements_.rbegin(), elements_.rend(),
                     IsVariantType<AdmElementId, ElementIdVariant>());

    if (foundLast == elements_.rend()) {
      throw std::logic_error("No element id of given type found");
    }
    return boost::get<AdmElementId>(*foundLast);
  }

  template <typename AdmElementId>
  bool Path::hasIdType() const {
    auto found = std::find_if(elements_.begin(), elements_.end(),
                              IsVariantType<AdmElementId, ElementIdVariant>());
    if (found != elements_.end()) {
      return true;
    } else {
      return false;
    }
  }

  template <typename Element>
  std::shared_ptr<const Element> getLastOf(const Path& path,
                                           const Frame& frame) {
    using ElementId = typename Element::id_type;
    return frame.lookup(path.getLastOf<ElementId>());
  }

  template <typename Element>
  std::shared_ptr<const Element> getFirstOf(const Path& path,
                                            const Frame& frame) {
    using ElementId = typename Element::id_type;
    return frame.lookup(path.getFirstOf<ElementId>());
  }

  inline bool operator==(const Path& lhs, const Path& rhs) {
    return lhs.hash() == rhs.hash() && lhs.elements_ == rhs.elements_;
  }

  inline bool operator!=(const Path& lhs, const Path& rhs) {
    return !(lhs == rhs);
  }

  inline bool operator<(const Path& lhs, const Path& rhs) {
    if (lhs.hash() != rhs.hash()) {
      return lhs.hash() < rhs.hash();
    } else {
      return std::lexicographical_compare(
          lhs.elements_.begin(), lhs.elements_.end(), rhs.elements_.begin(),
          rhs.elements_.end());
    }
  }

}  // namespace adm
