#include <catch2/catch.hpp>
#include "adm/elements.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/route_tracer.hpp"

using namespace adm;

TEST_CASE("basic") {
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto content = AudioContent::create(AudioContentName("Content"));
  programme->addReference(content);
  auto holder = createSimpleObject("Object");
  content->addReference(holder.audioObject);

  RouteTracer tracer;
  auto routes = tracer.run(programme);
  REQUIRE(routes.size() == 1);
  Route &route = routes[0];

  Route expected_route;
  expected_route.add(programme);
  expected_route.add(content);
  expected_route.add(holder.audioObject);
  expected_route.add(holder.audioPackFormat);
  expected_route.add(holder.audioChannelFormat);

  REQUIRE(route == expected_route);
}

// strategy following the route
// programme->content->object->trackuid->trackformat->streamformat->channelformat
struct FullDepthViaUIDStrategy {
  template <typename SubElement, typename Element>
  bool shouldRecurse(std::shared_ptr<Element>, std::shared_ptr<SubElement>) {
    return false;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioProgramme>,
                     std::shared_ptr<const adm::AudioContent>) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioContent>,
                     std::shared_ptr<const adm::AudioObject>) {
    return true;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioObject>,
                     std::shared_ptr<const adm::AudioObject>) {
    return true;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioObject>,
                     std::shared_ptr<const adm::AudioTrackUid>) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioTrackUid>,
                     std::shared_ptr<const adm::AudioTrackFormat>) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioTrackFormat>,
                     std::shared_ptr<const adm::AudioStreamFormat>) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioStreamFormat>,
                     std::shared_ptr<const adm::AudioChannelFormat>) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioTrackUid>,
                     std::shared_ptr<const adm::AudioChannelFormat>) {
    return true;
  }

  template <typename Element>
  bool shouldAdd(std::shared_ptr<Element>) {
    return true;
  }

  template <typename Element>
  bool isEndOfRoute(std::shared_ptr<Element>) {
    return false;
  }

  bool isEndOfRoute(std::shared_ptr<const adm::AudioChannelFormat>) {
    return true;
  }

  bool isEndOfRoute(
      const std::shared_ptr<const adm::AudioTrackUid> &track_uid) {
    return track_uid->isSilent();
  }
};

TEST_CASE("via_uid") {
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto content = AudioContent::create(AudioContentName("Content"));
  programme->addReference(content);
  auto holder = createSimpleObject("Object");
  content->addReference(holder.audioObject);

  detail::GenericRouteTracer<adm::Route, FullDepthViaUIDStrategy> tracer;
  auto routes = tracer.run(programme);
  REQUIRE(routes.size() == 1);
  Route &route = routes[0];

  Route expected_route;
  expected_route.add(programme);
  expected_route.add(content);
  expected_route.add(holder.audioObject);
  expected_route.add(holder.audioTrackUid);
  expected_route.add(holder.audioTrackFormat);
  expected_route.add(holder.audioStreamFormat);
  expected_route.add(holder.audioChannelFormat);

  REQUIRE(route == expected_route);
}

TEST_CASE("uid_to_channel_format") {
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));

  auto content = AudioContent::create(AudioContentName("Content"));
  programme->addReference(content);

  auto object = AudioObject::create(AudioObjectName("Object"));
  content->addReference(object);

  auto trackUid = AudioTrackUid::create();
  object->addReference(trackUid);

  auto channelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Channel Format"), TypeDefinition::OBJECTS);
  trackUid->setReference(channelFormat);

  detail::GenericRouteTracer<adm::Route, FullDepthViaUIDStrategy> tracer;
  auto routes = tracer.run(programme);
  REQUIRE(routes.size() == 1);
  Route &route = routes[0];

  Route expected_route;
  expected_route.add(programme);
  expected_route.add(content);
  expected_route.add(object);
  expected_route.add(trackUid);
  expected_route.add(channelFormat);

  REQUIRE(route == expected_route);
}

TEST_CASE("silent_track_uid") {
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));

  auto content = AudioContent::create(AudioContentName("Content"));
  programme->addReference(content);

  auto object = AudioObject::create(AudioObjectName("Object"));
  content->addReference(object);

  auto trackUid = AudioTrackUid::getSilent();
  object->addReference(trackUid);

  detail::GenericRouteTracer<adm::Route, FullDepthViaUIDStrategy> tracer;
  auto routes = tracer.run(programme);
  REQUIRE(routes.size() == 1);
  Route &route = routes[0];

  Route expected_route;
  expected_route.add(programme);
  expected_route.add(content);
  expected_route.add(object);
  expected_route.add(trackUid);

  REQUIRE(route == expected_route);
}
