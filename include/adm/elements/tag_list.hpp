#pragma once
#include <vector>
#include "adm/detail/auto_base.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/errors.hpp"

namespace adm {

  class TagList;

  struct TagValueTag {};
  using TagValue = detail::NamedType<std::string, TagValueTag>;

  struct TagClassTag {};
  using TagClass = detail::NamedType<std::string, TagClassTag>;

  struct TagTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<TagValue>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<TagClass>;

    using TagBase =
        HasParameters<RequiredParameter<TagValue>, OptionalParameter<TagClass>>;
  }  // namespace detail

  class Tag : private detail::TagBase, private detail::AddWrapperMethods<Tag> {
   public:
    using tag = TagTag;

    template <typename... Parameters>
    explicit Tag(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    ADM_EXPORT explicit Tag(std::string str) : Tag(TagValue(std::move(str))) {}
    ADM_EXPORT explicit Tag(const char *s);

    ADM_EXPORT void print(std::ostream &os) const;

    using detail::TagBase::set;
    using detail::TagBase::unset;
    using detail::AddWrapperMethods<Tag>::get;
    using detail::AddWrapperMethods<Tag>::has;
    using detail::AddWrapperMethods<Tag>::isDefault;
    using detail::AddWrapperMethods<Tag>::unset;

   private:
    using detail::TagBase::get;
    using detail::TagBase::has;

    friend class detail::AddWrapperMethods<Tag>;
  };

  struct TagsTag {};

  using Tags = std::vector<Tag>;
  ADD_TRAIT(Tags, TagsTag);

  inline bool operator==(const Tag &a, const Tag &b) {
    return detail::optionalsEqual<TagValue, TagClass>(a, b);
  }

  inline bool operator!=(const Tag &a, const Tag &b) { return !(a == b); }

  struct TagGroupTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<Tags>;

    using TagGroupBase = HasParameters<VectorParameter<Tags>>;
  }  // namespace detail

  class TagGroup : private detail::TagGroupBase,
                   private detail::AddWrapperMethods<TagGroup> {
   public:
    enum class RemoveResult {
      Success,
      LastReferenceError,  // A TagGroup must always have at least one reference
      NotFound
    };
    using tag = TagGroupTag;

    TagGroup() = default;

    template <typename... Parameters>
    explicit TagGroup(std::shared_ptr<AudioObject> const &reference,
                      Parameters... namedArgs) {
      addReference(reference);
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }
    template <typename... Parameters>
    explicit TagGroup(std::shared_ptr<AudioContent> const &reference,
                      Parameters... namedArgs) {
      addReference(reference);
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }
    template <typename... Parameters>
    explicit TagGroup(std::shared_ptr<AudioProgramme> const &reference,
                      Parameters... namedArgs) {
      addReference(reference);
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
    ADM_EXPORT RemoveResult
    removeReference(std::shared_ptr<AudioProgramme> programme);

    /// @brief Remove reference to an AudioContent
    ADM_EXPORT RemoveResult
    removeReference(std::shared_ptr<AudioContent> content);

    /// @brief Remove reference to an AudioObject
    ADM_EXPORT RemoveResult
    removeReference(std::shared_ptr<AudioObject> object);

    template <typename Element>
    void clearReferences();

    using AddWrapperMethods::get;
    using AddWrapperMethods::has;
    using AddWrapperMethods::isDefault;
    using AddWrapperMethods::unset;
    using detail::TagGroupBase::add;
    using detail::TagGroupBase::remove;
    using detail::TagGroupBase::set;

    template <typename T>
    bool has() const {
      return has(typename detail::ParameterTraits<T>::tag{});
    }

   private:
    using detail::TagGroupBase::get;
    using detail::TagGroupBase::has;
    using detail::TagGroupBase::isDefault;
    using detail::TagGroupBase::unset;

    friend class detail::AddWrapperMethods<TagGroup>;
    friend class Document;
    friend class TagList;

    bool invalid() const;

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

    std::vector<std::shared_ptr<AudioProgramme>> audioProgrammes_;
    std::vector<std::shared_ptr<AudioContent>> audioContents_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
  };

  inline bool operator==(const TagGroup &a, const TagGroup &b) {
    return detail::optionalsEqual<Tags>(a, b) &&
           detail::elementRangeEqual<AudioObject const>(
               a.getReferences<AudioObject>(),
               b.getReferences<AudioObject>()) &&
           detail::elementRangeEqual<AudioProgramme>(
               a.getReferences<AudioProgramme>(),
               b.getReferences<AudioProgramme>()) &&
           detail::elementRangeEqual<AudioContent>(
               a.getReferences<AudioContent>(),
               b.getReferences<AudioContent>());
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
    return ElementRange<const AudioProgramme>(audioProgrammes_.begin(),
                                              audioProgrammes_.end());
  }

  inline ElementRange<const AudioContent> TagGroup::getReferences(
      detail::ParameterTraits<AudioContent>::tag) const {
    return ElementRange<const AudioContent>(audioContents_.begin(),
                                            audioContents_.end());
  }

  inline ElementRange<const AudioObject> TagGroup::getReferences(
      detail::ParameterTraits<AudioObject>::tag) const {
    return ElementRange<const AudioObject>(audioObjects_.begin(),
                                           audioObjects_.end());
  }

  inline ElementRange<AudioProgramme> TagGroup::getReferences(
      detail::ParameterTraits<AudioProgramme>::tag) {
    return ElementRange<AudioProgramme>(audioProgrammes_.begin(),
                                        audioProgrammes_.end());
  }

  inline ElementRange<AudioContent> TagGroup::getReferences(
      detail::ParameterTraits<AudioContent>::tag) {
    return ElementRange<AudioContent>(audioContents_.begin(),
                                      audioContents_.end());
  }

  inline ElementRange<AudioObject> TagGroup::getReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return ElementRange<AudioObject>(audioObjects_.begin(),
                                     audioObjects_.end());
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
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<TagGroups>;

    using TagListBase = HasParameters<VectorParameter<TagGroups>>;
  }  // namespace detail

  struct TagListTag {};

  class TagList : private detail::TagListBase,
                  private detail::AddWrapperMethods<TagList>,
                  public std::enable_shared_from_this<TagList> {
   public:
    template <typename... Parameters>
    std::shared_ptr<TagList> create(Parameters... namedArgs) {
      return std::make_shared<TagList>(
          std::forward<Parameters...>(namedArgs...));
    }
    using tag = TagListTag;
    using detail::TagListBase::set;
    using detail::AddWrapperMethods<TagList>::get;
    using detail::AddWrapperMethods<TagList>::has;
    using detail::AddWrapperMethods<TagList>::isDefault;
    using detail::AddWrapperMethods<TagList>::unset;
    using detail::TagListBase::remove;

    ADM_EXPORT bool add(TagGroup group);

    template <typename... Parameters>
    explicit TagList(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

   private:
    using detail::TagListBase::get;
    using detail::TagListBase::has;
    using detail::TagListBase::isDefault;
    using detail::TagListBase::unset;

    friend class detail::AddWrapperMethods<TagList>;
  };
}  // namespace adm
