#pragma once
#include <vector>
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {
  struct ProfileValueTag {};
  using ProfileValue = detail::NamedType<std::string, ProfileValueTag>;

  struct ProfileNameTag {};
  using ProfileName = detail::NamedType<std::string, ProfileNameTag>;

  struct ProfileVersionTag {};
  using ProfileVersion = detail::NamedType<std::string, ProfileVersionTag>;

  struct ProfileLevelTag {};
  using ProfileLevel = detail::NamedType<std::string, ProfileLevelTag>;

  struct ProfileTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<ProfileValue>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<ProfileName>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<ProfileVersion>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<ProfileLevel>;

    using ProfileBase = HasParameters<
        RequiredParameter<ProfileValue>, RequiredParameter<ProfileName>,
        RequiredParameter<ProfileVersion>, RequiredParameter<ProfileLevel>>;
  }  // namespace detail

  class Profile : private detail::ProfileBase,
                  private detail::AddWrapperMethods<Profile> {
   public:
    using tag = ProfileTag;

    template <typename... Parameters>
    explicit Profile(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
      static_assert(detail::optionInList<ProfileValue, Parameters...>(),
                    "ProfileValue must be specified");
      static_assert(detail::optionInList<ProfileName, Parameters...>(),
                    "ProfileName must be specified");
      static_assert(detail::optionInList<ProfileVersion, Parameters...>(),
                    "ProfileVersion must be specified");
      static_assert(detail::optionInList<ProfileLevel, Parameters...>(),
                    "ProfileLevel must be specified");
    }

    using detail::ProfileBase::set;
    using detail::AddWrapperMethods<Profile>::get;
    using detail::AddWrapperMethods<Profile>::has;
    using detail::AddWrapperMethods<Profile>::isDefault;
    using detail::AddWrapperMethods<Profile>::unset;

   private:
    using detail::ProfileBase::get;
    using detail::ProfileBase::has;

    friend class detail::AddWrapperMethods<Profile>;
  };

  inline bool operator==(const Profile &a, const Profile &b) {
    return detail::optionalsEqual<ProfileValue, ProfileName, ProfileVersion,
                                  ProfileLevel>(a, b);
  }

  inline bool operator!=(const Profile &a, const Profile &b) {
    return !(a == b);
  }

  struct ProfilesTag {};

  using Profiles = std::vector<Profile>;
  ADD_TRAIT(Profiles, ProfilesTag);

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<Profiles>;

    using ProfileListBase = HasParameters<VectorParameter<Profiles>>;
  }  // namespace detail

  struct ProfileceListTag {};

  class ProfileList : private detail::ProfileListBase,
                      private detail::AddWrapperMethods<ProfileList> {
   public:
    using tag = ProfileceListTag;

    template <typename... Parameters>
    explicit ProfileList(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::ProfileListBase::set;
    using detail::AddWrapperMethods<ProfileList>::get;
    using detail::AddWrapperMethods<ProfileList>::has;
    using detail::AddWrapperMethods<ProfileList>::isDefault;
    using detail::AddWrapperMethods<ProfileList>::unset;
    using detail::ProfileListBase::add;
    using detail::ProfileListBase::remove;

   private:
    using detail::ProfileListBase::get;
    using detail::ProfileListBase::has;
    using detail::ProfileListBase::isDefault;
    using detail::ProfileListBase::unset;

    friend class detail::AddWrapperMethods<ProfileList>;
  };
}  // namespace adm
