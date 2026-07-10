#include "adm/elements/tag_list.hpp"
#include <algorithm>

namespace adm {
  Tag::Tag(const char* s) {
    // to avoid UB from std::string
    if (!s) {
      throw error::AdmGenericRuntimeError{
          "Cannot construct Tag from null const char*"};
    }
    set(TagValue{std::string{s}});
  }

  template <typename T>
  bool add_reference(std::vector<std::shared_ptr<T>>& references,
                     std::shared_ptr<T> ref) {
    auto it = std::find(references.begin(), references.end(), ref);
    if (it == references.end()) {
      references.push_back(ref);
      return true;
    }
    return false;
  }

  // ---- References ---- //
  bool TagGroup::addReference(std::shared_ptr<AudioProgramme> programme) {
    return add_reference(audioProgrammes_, std::move(programme));
  }

  bool TagGroup::addReference(std::shared_ptr<AudioContent> content) {
    return add_reference(audioContents_, std::move(content));
  }

  bool TagGroup::addReference(std::shared_ptr<AudioObject> object) {
    return add_reference(audioObjects_, std::move(object));
  }

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

  bool TagList::add(TagGroup group) {
    return detail::TagListBase::add(std::move(group));
  }

  namespace detail {
    template class RequiredParameter<TagValue>;
    template class OptionalParameter<TagClass>;
    template class VectorParameter<Tags>;
    template class VectorParameter<TagGroups>;
  }  // namespace detail
}  // namespace adm
