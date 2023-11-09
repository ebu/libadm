#ifndef LIBADM_CHANGED_IDS_HPP
#define LIBADM_CHANGED_IDS_HPP
#include <adm/detail/auto_base.hpp>
#include <adm/elements.hpp>

namespace adm {
  enum class StatusValue { NEW, CHANGED, EXTENDED, EXPIRED };

  /// Tag for status
  struct StatusTag {};
  using Status = detail::NamedType<StatusValue, StatusTag>;

  std::string formatValue(StatusValue status);

  // useful for templated access

  namespace detail {
    template <typename T>
    using IDRefBase = HasParameters<RequiredParameter<typename T::id_type>,
                                    RequiredParameter<Status>>;
  }

  /**
   * @brief Template class for referring to any of the changedIDs sub-elements
   */
  template <typename T>
  class IDRef : private detail::IDRefBase<T>,
                private detail::AddWrapperMethods<IDRef<T>> {
   public:
    IDRef(typename T::id_type id, Status status) {
      if (id == typename T::id_type{}) {
        /* Not using shared pointers to elements as header is outside document.
        * This means we need to check IDs have been assigned
        * before they are added to the header
        */
        throw std::runtime_error(
            "Cannot reference a default ID - either add the referred element"
            "to a document or manually assign a valid ID value");
      }
      detail::IDRefBase<T>::set(id);
      detail::IDRefBase<T>::set(status);
    }

    void set(Status status) { detail::IDRefBase<T>::set(std::move(status)); }

    void set(typename T::id_type id) {}
    // no set to ensure ID check, doesn't really need to be mutable
    using detail::AddWrapperMethods<IDRef<T>>::get;
    using detail::AddWrapperMethods<IDRef<T>>::has;
    using detail::AddWrapperMethods<IDRef<T>>::isDefault;
    using detail::IDRefBase<T>::get;
    using detail::IDRefBase<T>::has;

    friend class detail::AddWrapperMethods<IDRef<T>>;
    friend bool operator==(IDRef<T> const& lhs, IDRef<T> const& rhs) {
      using id_t = typename T::id_type;
      return lhs.get<id_t>() == rhs.get<id_t>() &&
             lhs.get<Status>().get() == rhs.get<Status>().get();
    }
  };

  template <typename T>
  IDRef<T> createIdRef(std::shared_ptr<T> element, Status status) {
    return IDRef<T>{element->template get<typename T::id_type>(),
                    std::move(status)};
  }

  /// Tags for IdRefs
  struct AudioChannelFormatIdRefsTag {};
  struct AudioPackFormatIdRefsTag {};
  struct AudioTrackUidIdRefsTag {};
  struct AudioTrackFormatIdRefsTag {};
  struct AudioStreamFormatIdRefsTag {};
  struct AudioObjectIdRefsTag {};
  struct AudioContentIdRefsTag {};
  struct AudioProgrammeIdRefsTag {};

  using AudioChannelFormatIdRefs = std::vector<IDRef<AudioChannelFormat>>;
  using AudioPackFormatIdRefs = std::vector<IDRef<AudioPackFormat>>;
  using AudioTrackUidIdRefs = std::vector<IDRef<AudioTrackUid>>;
  using AudioTrackFormatIdRefs = std::vector<IDRef<AudioTrackFormat>>;
  using AudioStreamFormatIdRefs = std::vector<IDRef<AudioStreamFormat>>;
  using AudioObjectIdRefs = std::vector<IDRef<AudioObject>>;
  using AudioContentIdRefs = std::vector<IDRef<AudioContent>>;
  using AudioProgrammeIdRefs = std::vector<IDRef<AudioProgramme>>;

  /// Traits for IDRefs
  ADD_TRAIT(AudioChannelFormatIdRefs, AudioChannelFormatIdRefsTag)
  ADD_TRAIT(AudioPackFormatIdRefs, AudioPackFormatIdRefsTag)
  ADD_TRAIT(AudioTrackUidIdRefs, AudioTrackUidIdRefsTag)
  ADD_TRAIT(AudioTrackFormatIdRefs, AudioTrackFormatIdRefsTag)
  ADD_TRAIT(AudioStreamFormatIdRefs, AudioStreamFormatIdRefsTag)
  ADD_TRAIT(AudioObjectIdRefs, AudioObjectIdRefsTag)
  ADD_TRAIT(AudioContentIdRefs, AudioContentIdRefsTag)
  ADD_TRAIT(AudioProgrammeIdRefs, AudioProgrammeIdRefsTag)

  namespace detail {
    using ChangedIdsBase =
        HasParameters<VectorParameter<AudioChannelFormatIdRefs>,
                      VectorParameter<AudioContentIdRefs>,
                      VectorParameter<AudioPackFormatIdRefs>,
                      VectorParameter<AudioTrackUidIdRefs>,
                      VectorParameter<AudioTrackFormatIdRefs>,
                      VectorParameter<AudioStreamFormatIdRefs>,
                      VectorParameter<AudioObjectIdRefs>,
                      VectorParameter<AudioProgrammeIdRefs>>;
  }  // namespace detail

  struct ChangedIdsTag {};
  /**
   * @brief Class representation of the audioContent ADM element
   *
   * \rst
   * +--------------------------+----------------------------------+----------------------------+
   * | ADM Parameter            | Parameter Type                   | Pattern Type               |
   * +==========================+==================================+============================+
   * | status                   | :type:`Status`                   | :class:`RequiredParameter` |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioChannelFormatIdRef  | :type:`AudioChannelIdRefs`       | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioPackFormatIdRef     | :type:`AudioPackFormatIdRefs`    | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioTrackUIDRef         | :type:`AudioTrackUIDIdRefs`      | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioStreamFormatIDRef   | :type:`AudioStreamFormatIDRefs`  | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioObjectIDRef         | :type:`AudioObjectIDRefs`        | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioContentIDRef        | :type:`AudioContentIDRefs`       | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * | audioProgrammeIDRef      | :type:`AudioProgrammeIDRefs`     | :class:`VectorParameter`   |
   * +--------------------------+----------------------------------+----------------------------+
   * \endrst
   */
  class ChangedIds : public detail::ChangedIdsBase,
                     private detail::AddWrapperMethods<ChangedIds> {
   public:
    template <typename... Parameters>
    explicit ChangedIds(Parameters&&... parameters);

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

#endif  //LIBADM_CHANGED_IDS_HPP
