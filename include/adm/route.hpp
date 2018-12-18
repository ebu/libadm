#pragma once
#include "adm/element_variant.hpp"
#include "adm/elements.hpp"
#include <boost/functional/hash.hpp>
#include <vector>
#include <algorithm>
#include <memory>

namespace adm {

  /**
   * @brief Route along ADM elements within an ADM document
   *
   * Describes route along ADM elements within an ADM document, i.e.
   * which references were followed to reach a certain element from a
   * given start point.
   *
   * This is the same purpose (and basic interface) as `adm::Path`, but
   * currently the implementation and constraints are different.
   *
   * Thus, the name `adm::Route` has been chosen to distinguish between those
   * two.
   *
   * The main/essential difference is that the `adm::Route` stores pointers to
   * the ADM elements, to make it easy to access the elements after a
   * `adm::Route` has been created, while the `adm::Path` does not, as they
   * might become invalid. Instead the `adm::Path` stores the (coded) element
   * ids, which are also used to check if `adm::Path`s are equal or not.
   *
   * To easily create an `adm::Route` you may use the `adm::RouteTracer`.
   */

  class Route {
   public:
    typedef std::size_t hash_type;
    typedef std::vector<adm::ElementConstVariant>::iterator iterator;
    typedef std::vector<adm::ElementConstVariant>::const_iterator
        const_iterator;
    typedef adm::ElementConstVariant value_type;

    Route() = default;

    template <typename Element>
    void add(std::shared_ptr<Element> element) {
      route_.push_back(element);
      using ElementId = typename Element::id_type;
      auto id = element->template get<ElementId>();
      boost::hash_combine(hash_, adm::formatId(id));
    }

    void add(adm::ElementConstVariant element) {
      boost::apply_visitor(AddVisitor(*this), element);
    }

    template <typename Element>
    std::shared_ptr<const Element> getFirstOf() const;

    template <typename Element>
    std::shared_ptr<const Element> getLastOf() const;

    template <typename Element>
    const_iterator findFirstOf() const;
    template <typename Element>
    const_iterator findLastOf() const;

    const_iterator begin() const { return route_.begin(); }
    const_iterator end() const { return route_.end(); }

    const adm::ElementConstVariant& front() const { return route_.front(); }
    const adm::ElementConstVariant& back() const { return route_.back(); }

    std::size_t size() const { return route_.size(); }

    /// @brief Get hash value
    hash_type hash() const { return hash_; }

   private:
    struct AddVisitor : public boost::static_visitor<> {
      AddVisitor(Route& admRoute) : admRoute_(admRoute) {}

      template <typename Element>
      void operator()(std::shared_ptr<Element> element) const {
        admRoute_.add(element);
      }

     private:
      Route& admRoute_;
    };

    friend bool operator==(const Route& lhs, const Route& rhs);
    friend bool operator<(const Route& lhs, const Route& rhs);
    std::vector<adm::ElementConstVariant> route_;
    hash_type hash_ = 0;
  };

  template <typename Element>
  Route::const_iterator Route::findFirstOf() const {
    auto foundFirst = std::find_if(
        route_.begin(), route_.end(),
        IsVariantType<std::shared_ptr<const Element>, ElementConstVariant>());

    return foundFirst;
  };

  template <typename Element>
  std::shared_ptr<const Element> Route::getFirstOf() const {
    auto foundFirst = findFirstOf<Element>();
    if (foundFirst != route_.end()) {
      return boost::get<std::shared_ptr<const Element>>(*foundFirst);
    } else {
      return nullptr;
    }
  };

  template <typename Element>
  Route::const_iterator Route::findLastOf() const {
    auto foundLast = std::find_if(
        route_.rbegin(), route_.rend(),
        IsVariantType<std::shared_ptr<const Element>, ElementConstVariant>());

    // a little bit clumsy, but we need to convert our (internal) reverse
    // to an iterator that points to the correct element.
    if (foundLast == route_.rend()) {
      return route_.end();
    }
    return foundLast.base() - 1;
  };

  template <typename Element>
  std::shared_ptr<const Element> Route::getLastOf() const {
    const_iterator foundLast = findLastOf<Element>();
    if (foundLast != route_.end()) {
      return boost::get<std::shared_ptr<const Element>>(*foundLast);
    } else {
      return nullptr;
    }
  };

  inline bool operator==(const Route& lhs, const Route& rhs) {
    return lhs.hash() == rhs.hash() && lhs.route_ == rhs.route_;
  }

  inline bool operator!=(const Route& lhs, const Route& rhs) {
    return !(lhs == rhs);
  }

  inline bool operator<(const Route& lhs, const Route& rhs) {
    if (lhs.hash() != rhs.hash()) {
      return lhs.hash() < rhs.hash();
    } else {
      return std::lexicographical_compare(lhs.route_.begin(), lhs.route_.end(),
                                          rhs.route_.begin(), rhs.route_.end());
    }
  }
}  // namespace adm
