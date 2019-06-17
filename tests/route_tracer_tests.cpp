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
  bool shouldRecurse(std::shared_ptr<Element> a,
                     std::shared_ptr<SubElement> b) {
    return false;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioProgramme> a,
                     std::shared_ptr<const adm::AudioContent> b) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioContent> a,
                     std::shared_ptr<const adm::AudioObject> b) {
    return true;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioObject> a,
                     std::shared_ptr<const adm::AudioObject> b) {
    return true;
  }

  bool shouldRecurse(std::shared_ptr<const adm::AudioObject> a,
                     std::shared_ptr<const adm::AudioTrackUid> b) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioTrackUid> a,
                     std::shared_ptr<const adm::AudioTrackFormat> b) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioTrackFormat> a,
                     std::shared_ptr<const adm::AudioStreamFormat> b) {
    return true;
  }
  bool shouldRecurse(std::shared_ptr<const adm::AudioStreamFormat> a,
                     std::shared_ptr<const adm::AudioChannelFormat> b) {
    return true;
  }

  template <typename Element>
  bool shouldAdd(std::shared_ptr<Element>) {
    return true;
  }

  template <typename Element>
  bool isEndOfRoute(std::shared_ptr<Element> e) {
    return false;
  }

  bool isEndOfRoute(std::shared_ptr<const adm::AudioChannelFormat>) {
    return true;
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
