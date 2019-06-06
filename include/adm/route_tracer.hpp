#pragma once

#include "adm/elements.hpp"
#include "adm/route.hpp"
#include <memory>

namespace adm {
  namespace detail {

    struct DefaultFullDepthStrategy {
      template <typename SubElement, typename Element>
      bool shouldRecurse(std::shared_ptr<Element>,
                         std::shared_ptr<SubElement>) {
        return true;
      }
      bool shouldRecurse(std::shared_ptr<const AudioObject>,
                         std::shared_ptr<const AudioTrackUid>) {
        return false;
      }
      bool shouldRecurse(std::shared_ptr<const AudioStreamFormat>,
                         std::shared_ptr<const AudioChannelFormat>) {
        return false;
      }
      template <typename Element>
      bool shouldAdd(std::shared_ptr<Element>) {
        return true;
      }
      template <typename Element>
      bool isEndOfRoute(std::shared_ptr<Element>) {
        return false;
      }
      bool isEndOfRoute(std::shared_ptr<const AudioChannelFormat>) {
        return true;
      }
    };

    template <typename Route, class Strategy>
    class GenericRouteTracer : public Strategy {
     public:
      typedef std::vector<Route> result_type;
      GenericRouteTracer() = default;
      template <typename Element>
      std::vector<Route> run(std::shared_ptr<Element> startPoint) {
        admRoutes_.clear();
        trace(startPoint, Route());
        return admRoutes_;
      }

     private:
      void trace(std::shared_ptr<const AudioProgramme> audioProgramme,
                 Route admRoute) {
        if (this->shouldAdd(audioProgramme)) {
          admRoute.add(audioProgramme);
        }

        for (auto& subelement : audioProgramme->getReferences<AudioContent>()) {
          if (this->shouldRecurse(audioProgramme, subelement)) {
            trace(subelement, admRoute);
          }
        }
        if (this->isEndOfRoute(audioProgramme)) {
          admRoutes_.push_back(admRoute);
        }
      }
      void trace(std::shared_ptr<const AudioContent> audioContent,
                 Route admRoute) {
        if (this->shouldAdd(audioContent)) {
          admRoute.add(audioContent);
        }

        for (auto& subelement : audioContent->getReferences<AudioObject>()) {
          if (this->shouldRecurse(audioContent, subelement)) {
            trace(subelement, admRoute);
          }
        }
        if (this->isEndOfRoute(audioContent)) {
          admRoutes_.push_back(admRoute);
        }
      }

      void trace(std::shared_ptr<const AudioObject> audioObject,
                 Route admRoute) {
        if (this->shouldAdd(audioObject)) {
          admRoute.add(audioObject);
        }

        for (auto& subelement : audioObject->getReferences<AudioPackFormat>()) {
          if (this->shouldRecurse(audioObject, subelement)) {
            trace(subelement, admRoute);
          }
        }
        for (auto& subelement : audioObject->getReferences<AudioObject>()) {
          if (this->shouldRecurse(audioObject, subelement)) {
            trace(subelement, admRoute);
          }
        }
        for (auto& subelement : audioObject->getReferences<AudioTrackUid>()) {
          if (this->shouldRecurse(audioObject, subelement)) {
            trace(subelement, admRoute);
          }
        }
        if (this->isEndOfRoute(audioObject)) {
          admRoutes_.push_back(admRoute);
        }
      }

      void trace(std::shared_ptr<const AudioPackFormat> audioPackFormat,
                 Route admRoute) {
        if (this->shouldAdd(audioPackFormat)) {
          admRoute.add(audioPackFormat);
        }

        for (auto& subelement :
             audioPackFormat->getReferences<AudioChannelFormat>()) {
          if (this->shouldRecurse(audioPackFormat, subelement)) {
            trace(subelement, admRoute);
          }
        }

        for (auto& subelement :
             audioPackFormat->getReferences<AudioPackFormat>()) {
          if (this->shouldRecurse(audioPackFormat, subelement)) {
            trace(subelement, admRoute);
          }
        }
        if (this->isEndOfRoute(audioPackFormat)) {
          admRoutes_.push_back(admRoute);
        }
      }
      void trace(std::shared_ptr<const AudioChannelFormat> audioChannelFormat,
                 Route admRoute) {
        if (this->shouldAdd(audioChannelFormat)) {
          admRoute.add(audioChannelFormat);
        }
        if (this->isEndOfRoute(audioChannelFormat)) {
          admRoutes_.push_back(admRoute);
        }
      }

      void trace(std::shared_ptr<const AudioTrackUid> audioTrackUid,
                 Route admRoute) {
        if (this->shouldAdd(audioTrackUid)) {
          admRoute.add(audioTrackUid);
        }
        auto subpack = audioTrackUid->getReference<AudioPackFormat>();
        if (subpack && this->shouldRecurse(audioTrackUid, subpack)) {
          trace(subpack, admRoute);
        }

        auto subtrack = audioTrackUid->getReference<AudioTrackFormat>();
        if (subtrack && this->shouldRecurse(audioTrackUid, subtrack)) {
          trace(subtrack, admRoute);
        }
        if (this->isEndOfRoute(audioTrackUid)) {
          admRoutes_.push_back(admRoute);
        }
      }

      void trace(std::shared_ptr<const AudioTrackFormat> audioTrackFormat,
                 Route admRoute) {
        if (this->shouldAdd(audioTrackFormat)) {
          admRoute.add(audioTrackFormat);
        }
        auto subpack = audioTrackFormat->getReference<AudioStreamFormat>();
        if (subpack && this->shouldRecurse(audioTrackFormat, subpack)) {
          trace(subpack, admRoute);
        }
      }

      void trace(std::shared_ptr<const AudioStreamFormat> audioStreamFormat,
                 Route admRoute) {
        if (this->shouldAdd(audioStreamFormat)) {
          admRoute.add(audioStreamFormat);
        }

        auto subpack = audioStreamFormat->getReference<AudioPackFormat>();
        if (subpack && this->shouldRecurse(audioStreamFormat, subpack)) {
          trace(subpack, admRoute);
        }
        for (auto& weak_subtrack :
             audioStreamFormat->getAudioTrackFormatReferences()) {
          auto subtrack = weak_subtrack.lock();
          if (subtrack && this->shouldRecurse(audioStreamFormat, subtrack)) {
            trace(subpack, admRoute);
          }
        }

        auto subchannel = audioStreamFormat->getReference<AudioChannelFormat>();
        if (subchannel && this->shouldRecurse(audioStreamFormat, subchannel)) {
          trace(subchannel, admRoute);
        }
      }

      std::vector<Route> admRoutes_;
    };
  }  // namespace detail

  /**
   * @brief Creates `adm::Route`s
   *
   * This implementation traces the following route:
   *
   *  - audioProgramme
   *  - audioContent
   *  - audioObject
   *  - audioPackFormat
   *  - audioChannelFormat
   *
   * Complementary AudioObjects are not interpreted as such. Hence for
   * every complementary audioObject an Route will be returned.
   *
   * @warning If the ADM structure contains a reference cycle, trace will get
   * stuck in an infinite loop.
   */
  using RouteTracer =
      detail::GenericRouteTracer<Route, detail::DefaultFullDepthStrategy>;

}  // namespace adm
