#include "adm/elements/tag_list.hpp"

namespace adm {
  TTag::TTag(const char* s) {
    // to avoid UB from std::string
    if (!s) {
      throw error::AdmGenericRuntimeError{
          "Cannot construct Tag from null const char*"};
    }
    set(TTagValue{std::string{s}});
  }

  // ---- References ---- //
  bool TagGroup::addReference(std::shared_ptr<AudioProgramme> programme) {
    auto it =
        std::find(audioProgrammes_.begin(), audioProgrammes_.end(), programme);
    if (it == audioProgrammes_.end()) {
      audioProgrammes_.push_back(std::move(programme));
      return true;
    } else {
      return false;
    }
  }

  bool TagGroup::addReference(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it == audioContents_.end()) {
      audioContents_.push_back(std::move(content));
      return true;
    } else {
      return false;
    }
  }

  bool TagGroup::addReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
      audioObjects_.push_back(std::move(object));
      return true;
    } else {
      return false;
    }
  }

  void TagGroup::removeReference(std::shared_ptr<AudioProgramme> programme) {
    auto it =
        std::find(audioProgrammes_.begin(), audioProgrammes_.end(), programme);
    if (it != audioProgrammes_.end()) {
      audioProgrammes_.erase(it);
    }
  }

  void TagGroup::removeReference(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it != audioContents_.end()) {
      audioContents_.erase(it);
    }
  }

  void TagGroup::removeReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
    }
  }

  void TagGroup::disconnectReferences() {
    clearReferences<AudioProgramme>();
    clearReferences<AudioContent>();
    clearReferences<AudioObject>();
  }

  void TagGroup::clearReferences(detail::ParameterTraits<AudioProgramme>::tag) {
    audioProgrammes_.clear();
  }

  void TagGroup::clearReferences(detail::ParameterTraits<AudioContent>::tag) {
    audioContents_.clear();
  }

  void TagGroup::clearReferences(detail::ParameterTraits<AudioObject>::tag) {
    audioObjects_.clear();
  }

  namespace detail {
    template class RequiredParameter<TTagValue>;
    template class OptionalParameter<TTagClass>;
    template class VectorParameter<TTags>;
    template class VectorParameter<TagGroups>;
  }  // namespace detail
}  // namespace adm
