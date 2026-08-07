#include "adm/elements/loudness_renderer.hpp"
#include "adm/elements/authoring_information.hpp"
#include "adm/document.hpp"

#include <algorithm>
#include <stdexcept>

namespace adm {

  LoudnessRenderer LoudnessRenderer::fromRenderer(
      AuthoringRenderer const& renderer) {
    LoudnessRenderer loudnessRenderer;
    if (renderer.has<RendererUri>()) {
      loudnessRenderer.set(renderer.get<RendererUri>());
    }
    if (renderer.has<RendererName>()) {
      loudnessRenderer.set(renderer.get<RendererName>());
    }
    if (renderer.has<RendererVersion>()) {
      loudnessRenderer.set(renderer.get<RendererVersion>());
    }
    if (renderer.has<CoordinateMode>()) {
      loudnessRenderer.set(renderer.get<CoordinateMode>());
    }
    auto refs = renderer.getReferences<AudioPackFormat>();
    if (!refs.empty()) {
      loudnessRenderer.set(
          std::const_pointer_cast<AudioPackFormat>(refs.front()));
    }
    return loudnessRenderer;
  }

  bool LoudnessRenderer::addReference(std::shared_ptr<AudioObject> object) {
    auto rendererParent = parent_.lock();
    auto objectParent = object->getParent().lock();
    if (rendererParent) {
      if (objectParent && objectParent != rendererParent) {
        throw std::runtime_error(
            "LoudnessRenderer cannot refer to an AudioObject in a different "
            "document");
      }
      if (!objectParent) {
        rendererParent->add(object);
      }
    }

    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
      audioObjects_.push_back(std::move(object));
      return true;
    }
    return false;
  }

  void LoudnessRenderer::removeReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
    }
  }

  ElementRange<AudioObject> LoudnessRenderer::getReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  ElementRange<const AudioObject> LoudnessRenderer::getReferences(
      detail::ParameterTraits<AudioObject>::tag) const {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  void LoudnessRenderer::clearReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    audioObjects_.clear();
  }

  void LoudnessRenderer::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error(
          "LoudnessRenderer already belongs to another Document");
    }

    if (newParent) {
      for (auto const& object : audioObjects_) {
        auto objectParent = object->getParent().lock();
        if (objectParent && objectParent != newParent) {
          throw std::runtime_error(
              "LoudnessRenderer cannot refer to an AudioObject in a "
              "different document");
        }
      }
      for (auto const& object : audioObjects_) {
        if (!object->getParent().lock()) {
          newParent->add(object);
        }
      }
    }

    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& LoudnessRenderer::getParent() const {
    return parent_;
  }

  AuthoringRenderer LoudnessRenderer::toRendererDroppingObjectRefs(
      RendererUri uri) const {
    AuthoringRenderer renderer{std::move(uri)};
    if (has<RendererUri>()) {
      renderer.set(get<RendererUri>());
    }
    if (has<RendererName>()) {
      renderer.set(get<RendererName>());
    }
    if (has<RendererVersion>()) {
      renderer.set(get<RendererVersion>());
    }
    if (has<CoordinateMode>()) {
      renderer.set(get<CoordinateMode>());
    }
    if (has<RendererPackFormatIdRef>()) {
      renderer.addReference(get<RendererPackFormatIdRef>());
    }
    return renderer;
  }

  void LoudnessRenderer::print(std::ostream& os) const {
    os << "(";
    bool first = true;
    auto sep = [&]() {
      if (!first) os << ", ";
      first = false;
    };
    if (has<RendererUri>()) {
      sep();
      os << "uri=" << get<RendererUri>();
    }
    if (has<RendererName>()) {
      sep();
      os << "name=" << get<RendererName>();
    }
    if (has<RendererVersion>()) {
      sep();
      os << "version=" << get<RendererVersion>();
    }
    if (has<CoordinateMode>()) {
      sep();
      os << "coordinateMode=" << get<CoordinateMode>();
    }
    os << ")";
  }

  namespace detail {
    template class OptionalParameter<RendererUri>;
    template class OptionalParameter<RendererName>;
    template class OptionalParameter<RendererVersion>;
    template class OptionalParameter<CoordinateMode>;
    template class OptionalParameter<RendererPackFormatIdRef>;
  }  // namespace detail

}  // namespace adm
