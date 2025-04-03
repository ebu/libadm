#pragma once
#include <vector>
#include "adm/detail/auto_base.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/errors.hpp"

namespace adm {
  struct TTagValueTag {};
  using TTagValue = detail::NamedType<std::string, TTagValueTag>;

  struct TTagClassTag {};
  using TTagClass = detail::NamedType<std::string, TTagClassTag>;

  struct TTagTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<TTagValue>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS OptionalParameter<TTagClass>;

    using TTagBase = HasParameters<RequiredParameter<TTagValue>,
                                   OptionalParameter<TTagClass>>;
  }  // namespace detail

  class TTag : private detail::TTagBase,
               private detail::AddWrapperMethods<TTag> {
   public:
    using tag = TTagTag;

    template <typename... Parameters>
    explicit TTag(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    ADM_EXPORT explicit TTag(std::string str)
        : TTag(TTagValue(std::move(str))) {}
    ADM_EXPORT explicit TTag(const char* s);

    ADM_EXPORT void print(std::ostream& os) const;

    using detail::TTagBase::set;
    using detail::TTagBase::unset;
    using detail::AddWrapperMethods<TTag>::get;
    using detail::AddWrapperMethods<TTag>::has;
    using detail::AddWrapperMethods<TTag>::isDefault;
    using detail::AddWrapperMethods<TTag>::unset;

   private:
    using detail::TTagBase::get;
    using detail::TTagBase::has;

    friend class detail::AddWrapperMethods<TTag>;
  };

  struct TTagsTag {};

  using TTags = std::vector<TTag>;
  ADD_TRAIT(TTags, TTagsTag);

  inline bool operator==(const TTag &a, const TTag &b) {
    return detail::optionalsEqual<TTagValue, TTagClass>(a, b);
  }

  inline bool operator!=(const TTag &a, const TTag &b) {
    return !(a==b);
  }

  struct TagGroupTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<TTags>;

    using TagGroupBase = HasParameters<VectorParameter<TTags>>;
  }  // namespace detail

  class TagGroup : private detail::TagGroupBase,
                  private detail::AddWrapperMethods<TagGroup> {
   public:
    using tag = TagGroupTag;

    // DEBUG FUNCTIONS
    int getTempId() { return temp_id_; };
    void setTempId(int n) { temp_id_ = n; };

    template <typename... Parameters>
    explicit TagGroup(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    /// @brief Add reference to an AudioProgramme
    ADM_EXPORT bool addReference(std::shared_ptr<AudioProgramme> programme);

    /// @brief Add reference to an AudioContent
    ADM_EXPORT bool addReference(std::shared_ptr<AudioContent> content);

    /// @brief Add reference to an AudioObject
    ADM_EXPORT bool addReference(std::shared_ptr<AudioObject> object);

    template <typename Element>
    ElementRange<Element> getReferences();

    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /// @brief Remove reference to an AudioProgramme
    ADM_EXPORT void removeReference(std::shared_ptr<AudioProgramme> programme);

    /// @brief Remove reference to an AudioContent
    ADM_EXPORT void removeReference(std::shared_ptr<AudioContent> content);

    /// @brief Remove reference to an AudioObject
    ADM_EXPORT void removeReference(std::shared_ptr<AudioObject> object);

    template <typename Element>
    void clearReferences();

    using detail::TagGroupBase::set;
    using detail::AddWrapperMethods<TagGroup>::get;
    using detail::AddWrapperMethods<TagGroup>::has;
    using detail::AddWrapperMethods<TagGroup>::isDefault;
    using detail::AddWrapperMethods<TagGroup>::unset;
    using detail::TagGroupBase::add;
    using detail::TagGroupBase::remove;

   private:
    int temp_id_;

    using detail::TagGroupBase::get;
    using detail::TagGroupBase::has;
    using detail::TagGroupBase::isDefault;
    using detail::TagGroupBase::unset;

    friend class detail::AddWrapperMethods<TagGroup>;

    ADM_EXPORT ElementRange<const AudioProgramme> getReferences(
        detail::ParameterTraits<AudioProgramme>::tag) const;
    ADM_EXPORT ElementRange<AudioProgramme> getReferences(
        detail::ParameterTraits<AudioProgramme>::tag);
    ADM_EXPORT ElementRange<const AudioContent> getReferences(
        detail::ParameterTraits<AudioContent>::tag) const;
    ADM_EXPORT ElementRange<AudioContent> getReferences(
        detail::ParameterTraits<AudioContent>::tag);
    ADM_EXPORT ElementRange<const AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag) const;
    ADM_EXPORT ElementRange<AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag);

    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioProgramme>::tag);
    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioContent>::tag);
    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioObject>::tag);

    ADM_EXPORT void disconnectReferences();

    std::vector<std::shared_ptr<AudioProgramme>> audioProgrammes_;
    std::vector<std::shared_ptr<AudioContent>> audioContents_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
  };

  inline bool operator==(const TagGroup &a, const TagGroup &b) {
    return detail::optionalsEqual<TTags>(a, b);
  }

  inline bool operator!=(const TagGroup &a, const TagGroup &b) {
    return !(a == b);
  }

  template <typename Element>
  ElementRange<const Element> TagGroup::getReferences() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<Element> TagGroup::getReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<const AudioProgramme> TagGroup::getReferences(
      detail::ParameterTraits<AudioProgramme>::tag) const {
    return detail::makeElementRange<AudioProgramme>(audioProgrammes_);
  }

  inline ElementRange<AudioProgramme> TagGroup::getReferences(
      detail::ParameterTraits<AudioProgramme>::tag) {
    return detail::makeElementRange<AudioProgramme>(audioProgrammes_);
  }

  inline ElementRange<const AudioContent> TagGroup::getReferences(
      detail::ParameterTraits<AudioContent>::tag) const {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  inline ElementRange<AudioContent> TagGroup::getReferences(
      detail::ParameterTraits<AudioContent>::tag) {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  inline ElementRange<const AudioObject> TagGroup::getReferences(
      detail::ParameterTraits<AudioObject>::tag) const {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  inline ElementRange<AudioObject> TagGroup::getReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  template <typename Element>
  void TagGroup::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

  struct TagGroupsTag {};

  using TagGroups = std::vector<TagGroup>;
  ADD_TRAIT(TagGroups, TagGroupsTag);

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<TagGroups>;

    using TagListBase = HasParameters<VectorParameter<TagGroups>>;
  }  // namespace detail

  struct TagListTag {};

  class TagList : private detail::TagListBase,
                  private detail::AddWrapperMethods<TagList> {
   public:
    using tag = TagListTag;

    template <typename... Parameters>
    explicit TagList(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::TagListBase::set;
    using detail::AddWrapperMethods<TagList>::get;
    using detail::AddWrapperMethods<TagList>::has;
    using detail::AddWrapperMethods<TagList>::isDefault;
    using detail::AddWrapperMethods<TagList>::unset;
    using detail::TagListBase::add;
    using detail::TagListBase::remove;

   private:
    using detail::TagListBase::get;
    using detail::TagListBase::has;
    using detail::TagListBase::isDefault;
    using detail::TagListBase::unset;

    friend class detail::AddWrapperMethods<TagList>;
  };
}  // namespace adm
