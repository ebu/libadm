/// @file changed_ids.hpp
#pragma once
#include <adm/detail/auto_base.hpp>
#include <adm/elements.hpp>

namespace adm {
  enum class ChangedIdStatus { NEW, CHANGED, EXTENDED, EXPIRED };

  /// Tag for status
  struct ChangedIdStatusTag {};
  namespace detail {
    template <>
    struct ParameterTraits<ChangedIdStatus> {
      using tag = ChangedIdStatusTag;
    };
  }  // namespace detail

  ADM_EXPORT std::string formatValue(ChangedIdStatus status);

  // useful for templated access

  namespace detail {
    template <typename T>
    using ChangedIdBase = HasParameters<RequiredParameter<typename T::id_type>,
                                        RequiredParameter<ChangedIdStatus>>;
  }

  /**
   * @brief Template class for referring to any of the changedIDs sub-elements
   * @tparam T the ADM element type to which the change refers.
   *
   * \rst
   * +---------------------------+--------------------------------------+---------------------------+
   * | ADM Parameter             | Parameter Type                       | Pattern Type              |
   * +===========================+======================================+===========================+
   * | status                    | :type:`ChangedIdStatus`              | :class:`RequiredParameter`|
   * +---------------------------+--------------------------------------+---------------------------+
   * | ADM element ID type for T | :type:`T::id_type`                   | :class:`RequiredParameter`|
   * +---------------------------+--------------------------------------+---------------------------+
   * \endrst
   */
  template <typename T>
  class ChangedId : private detail::ChangedIdBase<T>,
                    private detail::AddWrapperMethods<ChangedId<T>> {
   public:
    ADM_EXPORT ChangedId(typename T::id_type id, ChangedIdStatus status) {
      if (id == typename T::id_type{}) {
        /* Not using shared pointers to elements as header is outside document.
        * This means we need to check IDs have been assigned
        * before they are added to the header
        */
        throw std::runtime_error(
            "Cannot reference a default ID - either add the referred element"
            "to a document or manually assign a valid ID value");
      }
      detail::ChangedIdBase<T>::set(id);
      detail::ChangedIdBase<T>::set(status);
    }

    // no set to ensure ID check, doesn't really need to be mutable
    using detail::AddWrapperMethods<ChangedId<T>>::get;
    using detail::AddWrapperMethods<ChangedId<T>>::has;
    using detail::AddWrapperMethods<ChangedId<T>>::isDefault;
    using detail::ChangedIdBase<T>::get;
    using detail::ChangedIdBase<T>::has;

    friend class detail::AddWrapperMethods<ChangedId<T>>;
    ADM_EXPORT friend bool operator==(ChangedId<T> const& lhs,
                                      ChangedId<T> const& rhs) {
      using id_t = typename T::id_type;
      return lhs.template get<id_t>() == rhs.template get<id_t>() &&
             lhs.template get<ChangedIdStatus>() ==
                 rhs.template get<ChangedIdStatus>();
    }
  };

  template <typename T>
  ChangedId<T> createChangedId(std::shared_ptr<T> element,
                               ChangedIdStatus status) {
    return ChangedId<T>{element->template get<typename T::id_type>(), status};
  }

  /// Tags for IdRefs
  struct ChangedAudioChannelFormatIdsTag {};
  struct ChangedAudioPackFormatIdsTag {};
  struct ChangedAudioTrackUidIdsTag {};
  struct ChangedAudioTrackFormatIdsTag {};
  struct ChangedAudioStreamFormatIdsTag {};
  struct ChangedAudioObjectIdsTag {};
  struct ChangedAudioContentIdsTag {};
  struct ChangedAudioProgrammeIdsTag {};

  using ChangedAudioChannelFormatIds =
      std::vector<ChangedId<AudioChannelFormat>>;
  using ChangedAudioPackFormatIds = std::vector<ChangedId<AudioPackFormat>>;
  using ChangedAudioTrackUidIds = std::vector<ChangedId<AudioTrackUid>>;
  using ChangedAudioTrackFormatIds = std::vector<ChangedId<AudioTrackFormat>>;
  using ChangedAudioStreamFormatIds = std::vector<ChangedId<AudioStreamFormat>>;
  using ChangedAudioObjectIds = std::vector<ChangedId<AudioObject>>;
  using ChangedAudioContentIds = std::vector<ChangedId<AudioContent>>;
  using ChangedAudioProgrammeIds = std::vector<ChangedId<AudioProgramme>>;

  /// Traits for IDRefs
  ADD_TRAIT(ChangedAudioChannelFormatIds, ChangedAudioChannelFormatIdsTag)
  ADD_TRAIT(ChangedAudioPackFormatIds, ChangedAudioPackFormatIdsTag)
  ADD_TRAIT(ChangedAudioTrackUidIds, ChangedAudioTrackUidIdsTag)
  ADD_TRAIT(ChangedAudioTrackFormatIds, ChangedAudioTrackFormatIdsTag)
  ADD_TRAIT(ChangedAudioStreamFormatIds, ChangedAudioStreamFormatIdsTag)
  ADD_TRAIT(ChangedAudioObjectIds, ChangedAudioObjectIdsTag)
  ADD_TRAIT(ChangedAudioContentIds, ChangedAudioContentIdsTag)
  ADD_TRAIT(ChangedAudioProgrammeIds, ChangedAudioProgrammeIdsTag)

  namespace detail {
    using ChangedIdsBase =
        HasParameters<VectorParameter<ChangedAudioChannelFormatIds>,
                      VectorParameter<ChangedAudioContentIds>,
                      VectorParameter<ChangedAudioPackFormatIds>,
                      VectorParameter<ChangedAudioTrackUidIds>,
                      VectorParameter<ChangedAudioTrackFormatIds>,
                      VectorParameter<ChangedAudioStreamFormatIds>,
                      VectorParameter<ChangedAudioObjectIds>,
                      VectorParameter<ChangedAudioProgrammeIds>>;
  }  // namespace detail

  struct ChangedIdsTag {};
  /**
   * @brief Class representation of the changedIDs SADM element
   *
   * \rst
   * +--------------------------+--------------------------------------+---------------------------+
   * | ADM Parameter            | Parameter Type                       | Pattern Type              |
   * +==========================+======================================+===========================+
   * | audioChannelFormatIdRef  | :type:`ChangedAudioChannelFormatIds` | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioPackFormatIdRef     | :type:`ChangedAudioPackFormatIds`    | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioTrackUIDRef         | :type:`ChangedAudioTrackUidIds`      | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioStreamFormatIDRef   | :type:`ChangedAudioStreamFormatIds`  | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioObjectIDRef         | :type:`ChangedAudioObjectIds`        | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioContentIDRef        | :type:`ChangedAudioContentIds`       | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * | audioProgrammeIDRef      | :type:`ChangedAudioProgrammeIds`     | :class:`VectorParameter`  |
   * +--------------------------+--------------------------------------+---------------------------+
   * \endrst
   * @note type names are different to amd sub-element name, to avoid confusion with the similarly named
   * audioTrackUidRef element in audioTrack, and to reflect the fact it encapsulates id and status rather
   * than just the id.
   */
  class ChangedIds : private detail::ChangedIdsBase,
                     private detail::AddWrapperMethods<ChangedIds> {
   public:
    template <typename... Parameters>
    explicit ChangedIds(Parameters&&... parameters);

    template <typename ElementT>
    void add(std::shared_ptr<ElementT> const& element, ChangedIdStatus status) {
      add(createChangedId(element, status));
    }

    using tag = ChangedIdsTag;
    using detail::ChangedIdsBase::add;
    using detail::ChangedIdsBase::get;
    using detail::ChangedIdsBase::has;
    using detail::ChangedIdsBase::isDefault;
    using detail::ChangedIdsBase::remove;
    using detail::ChangedIdsBase::set;
    using detail::ChangedIdsBase::unset;
    using detail::AddWrapperMethods<ChangedIds>::get;
    using detail::AddWrapperMethods<ChangedIds>::unset;
    using detail::AddWrapperMethods<ChangedIds>::has;
    using detail::AddWrapperMethods<ChangedIds>::isDefault;

    friend class detail::AddWrapperMethods<ChangedIds>;
  };

  template <typename... Parameters>
  ChangedIds::ChangedIds(Parameters&&... parameters) {
    detail::setNamedOptionHelper(this, std::forward<Parameters>(parameters)...);
  }
}  // namespace adm
