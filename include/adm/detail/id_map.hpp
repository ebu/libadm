#pragma once
#include "adm/document.hpp"
#include <memory>

namespace adm {
  namespace detail {

    /// store a value for each top-level element type, with access by type
    ///
    /// the value is given by the template T, so that get<AudioProgramme>()
    /// returns a T<AudioProgramme>, for example
    template <template <typename Element> class T>
    struct ForEachElement {
      /// get one of the stored values
      template <typename El>
      T<El> &get() {
        return getTag(typename El::tag{});
      }

      /// call f on each of the stored values
      template <typename F>
      void visit(F f) {
        f(programmes);
        f(contents);
        f(objects);
        f(packFormats);
        f(channelFormats);
        f(streamFormats);
        f(trackFormats);
        f(trackUids);
      }

     private:
      T<AudioProgramme> &getTag(AudioProgramme::tag) { return programmes; }
      T<AudioContent> &getTag(AudioContent::tag) { return contents; }
      T<AudioObject> &getTag(AudioObject::tag) { return objects; }
      T<AudioPackFormat> &getTag(AudioPackFormat::tag) { return packFormats; }
      T<AudioChannelFormat> &getTag(AudioChannelFormat::tag) {
        return channelFormats;
      }
      T<AudioStreamFormat> &getTag(AudioStreamFormat::tag) {
        return streamFormats;
      }
      T<AudioTrackFormat> &getTag(AudioTrackFormat::tag) {
        return trackFormats;
      }
      T<AudioTrackUid> &getTag(AudioTrackUid::tag) { return trackUids; }

      T<AudioProgramme> programmes;
      T<AudioContent> contents;
      T<AudioObject> objects;
      T<AudioPackFormat> packFormats;
      T<AudioChannelFormat> channelFormats;
      T<AudioStreamFormat> streamFormats;
      T<AudioTrackFormat> trackFormats;
      T<AudioTrackUid> trackUids;
    };

    template <typename Id>
    struct IdToElement;

    template <>
    struct IdToElement<AudioProgrammeId> {
      using type = AudioProgramme;
    };
    template <>
    struct IdToElement<AudioContentId> {
      using type = AudioContent;
    };
    template <>
    struct IdToElement<AudioObjectId> {
      using type = AudioObject;
    };
    template <>
    struct IdToElement<AudioPackFormatId> {
      using type = AudioPackFormat;
    };
    template <>
    struct IdToElement<AudioChannelFormatId> {
      using type = AudioChannelFormat;
    };
    template <>
    struct IdToElement<AudioStreamFormatId> {
      using type = AudioStreamFormat;
    };
    template <>
    struct IdToElement<AudioTrackFormatId> {
      using type = AudioTrackFormat;
    };
    template <>
    struct IdToElement<AudioTrackUidId> {
      using type = AudioTrackUid;
    };

    /// mapping from ID type (e.g. AudioProgrammeId) to it's element type (e.g. AudioProgramme)
    template <typename Id>
    using IdToElementT = typename IdToElement<Id>::type;

    /// a mapping from IDs to elements for a particular type
    template <typename Element>
    class IDMapForElement {
      using Id = typename Element::id_type;

     public:
      IDMapForElement() {}

      IDMapForElement(Document &document) { update(document); }

      /// update the mapping to match the given document
      void update(Document &document) {
        id_to_element.clear();
        for (const auto &el : document.getElements<Element>())
          id_to_element.emplace(el->template get<Id>(), el);
      }

      /// add a new element
      void add(std::shared_ptr<Element> element) {
        id_to_element.emplace(element->template get<Id>(), std::move(element));
      }

      /// get an element with a given ID
      std::shared_ptr<Element> lookup(const Id &id) const {
        auto it = id_to_element.find(id);
        if (it != id_to_element.end())
          return it->second;
        else
          return {};
      }

      /// does the map have an element with a given ID?
      bool contains(const Id &id) const {
        return id_to_element.find(id) != id_to_element.end();
      }

     private:
      std::map<Id, std::shared_ptr<Element>> id_to_element;
    };

    /// mapping from IDs to elements for all top-level element types
    class IDMap {
     public:
      IDMap() {}

      IDMap(Document &doc) { update(doc); }

      /// update the mapping to match the given document
      void update(Document &doc) {
        idMaps.visit([&doc](auto &idMap) { idMap.update(doc); });
      }

      /// add a new element
      template <typename Element>
      void add(std::shared_ptr<Element> element) {
        idMaps.template get<Element>().add(std::move(element));
      }

      /// get an element with a given ID
      template <typename Id>
      auto lookup(const Id &id) {
        using Element = IdToElementT<Id>;
        return idMaps.template get<Element>().lookup(id);
      }

      /// does the map have an element with a given ID?
      template <typename Id>
      bool contains(const Id &id) {
        using Element = IdToElementT<Id>;
        return idMaps.template get<Element>().contains(id);
      }

     private:
      ForEachElement<IDMapForElement> idMaps;
    };

  }  // namespace detail
}  // namespace adm
