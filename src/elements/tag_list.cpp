#include "adm/elements/tag_list.hpp"
#include "adm/document.hpp"

#include <algorithm>
#include <stdexcept>

namespace adm {
  Tag::Tag(const char* s) {
    // to avoid UB from std::string
    if (!s) {
      throw error::AdmGenericRuntimeError{
          "Cannot construct Tag from null const char*"};
    }
    set(TagValue{std::string{s}});
  }

  void validate_tag_group_parent(TagGroup const& group,
                                 std::shared_ptr<Document> const& document) {
    auto groupParent = group.getParent().lock();
    if (groupParent && groupParent != document) {
      throw std::runtime_error("TagGroup already belongs to another Document");
    }

    auto validate = [&](auto const& references, const char* elementType) {
      for (auto const& reference : references) {
        auto referenceParent = reference->getParent().lock();
        if (referenceParent && referenceParent != document) {
          throw std::runtime_error(std::string{"TagGroup cannot refer to an "} +
                                   elementType + " in a different document");
        }
      }
    };
    validate(group.getReferences<AudioProgramme>(), "AudioProgramme");
    validate(group.getReferences<AudioContent>(), "AudioContent");
    validate(group.getReferences<AudioObject>(), "AudioObject");
  }

  template <typename T>
  bool add_reference(std::vector<std::shared_ptr<T>>& references,
                     std::weak_ptr<Document> const& parent,
                     std::shared_ptr<T> ref, const char* elementType) {
    if (auto document = parent.lock()) {
      auto referenceParent = ref->getParent().lock();
      if (referenceParent && referenceParent != document) {
        throw std::runtime_error(std::string{"TagGroup cannot refer to an "} +
                                 elementType + " in a different document");
      }
      if (!referenceParent) {
        document->add(ref);
      }
    }

    auto it = std::find(references.begin(), references.end(), ref);
    if (it == references.end()) {
      references.push_back(ref);
      return true;
    }
    return false;
  }

  // ---- References ---- //
  bool TagGroup::addReference(std::shared_ptr<AudioProgramme> programme) {
    return add_reference(audioProgrammes_, parent_, std::move(programme),
                         "AudioProgramme");
  }

  bool TagGroup::addReference(std::shared_ptr<AudioContent> content) {
    return add_reference(audioContents_, parent_, std::move(content),
                         "AudioContent");
  }

  bool TagGroup::addReference(std::shared_ptr<AudioObject> object) {
    return add_reference(audioObjects_, parent_, std::move(object),
                         "AudioObject");
  }

  const std::weak_ptr<Document>& TagGroup::getParent() const { return parent_; }

  template <typename T>
  TagGroup::RemoveResult remove_reference(
      std::vector<std::shared_ptr<T>>& references,
      std::shared_ptr<T> const& ref) {
    auto it = std::find(references.begin(), references.end(), ref);
    if (it == references.end()) {
      return TagGroup::RemoveResult::NotFound;
    }
    references.erase(it);
    return TagGroup::RemoveResult::Success;
  }

  TagGroup::RemoveResult TagGroup::removeReference(
      std::shared_ptr<AudioProgramme> programme) {
    if (remove_reference(audioProgrammes_, programme) ==
        RemoveResult::NotFound) {
      return RemoveResult::NotFound;
    }
    if (invalid()) {
      addReference(std::move(programme));
      return RemoveResult::LastReferenceError;
    }
    return RemoveResult::Success;
  }

  TagGroup::RemoveResult TagGroup::removeReference(
      std::shared_ptr<AudioContent> content) {
    if (remove_reference(audioContents_, content) == RemoveResult::NotFound) {
      return RemoveResult::NotFound;
    }
    if (invalid()) {
      addReference(std::move(content));
      return RemoveResult::LastReferenceError;
    }
    return RemoveResult::Success;
  }

  TagGroup::RemoveResult TagGroup::removeReference(
      std::shared_ptr<AudioObject> object) {
    if (remove_reference(audioObjects_, object) == RemoveResult::NotFound) {
      return RemoveResult::NotFound;
    }
    if (invalid()) {
      addReference(std::move(object));
      return RemoveResult::LastReferenceError;
    }
    return RemoveResult::Success;
  }

  bool TagGroup::invalid() const {
    return audioContents_.empty() && audioObjects_.empty() &&
           audioProgrammes_.empty();
  }

  void TagGroup::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error("TagGroup already belongs to another Document");
    }

    if (newParent) {
      validate_tag_group_parent(*this, newParent);

      auto adopt = [&](auto const& references) {
        for (auto const& reference : references) {
          if (!reference->getParent().lock()) {
            newParent->add(reference);
          }
        }
      };
      adopt(audioProgrammes_);
      adopt(audioContents_);
      adopt(audioObjects_);
    }

    parent_ = std::move(document);
  }

  bool TagList::add(TagGroup group) {
    if (auto parent = parent_.lock()) {
      group.setParent(parent);
    }
    return detail::TagListBase::add(std::move(group));
  }

  void TagList::set(TagGroups groups) {
    if (auto parent = parent_.lock()) {
      for (auto const& group : groups) {
        validate_tag_group_parent(group, parent);
      }
      for (auto& group : groups) {
        group.setParent(parent);
      }
    }
    detail::TagListBase::set(std::move(groups));
  }

  void TagList::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error("TagList already belongs to another Document");
    }

    auto groups = get<TagGroups>();
    if (newParent) {
      for (auto const& group : groups) {
        validate_tag_group_parent(group, newParent);
      }
      for (auto& group : groups) {
        group.setParent(newParent);
      }
    } else {
      for (auto& group : groups) {
        group.setParent(document);
      }
    }
    detail::TagListBase::set(std::move(groups));
    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& TagList::getParent() const { return parent_; }

  namespace detail {
    template class RequiredParameter<TagValue>;
    template class OptionalParameter<TagClass>;
    template class VectorParameter<Tags>;
    template class VectorParameter<TagGroups>;
  }  // namespace detail
}  // namespace adm
