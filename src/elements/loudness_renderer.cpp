#include "adm/elements/loudness_renderer.hpp"
#include "adm/elements/authoring_information.hpp"

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
    if (renderer.has<RendererPackFormatIdRefs>()) {
      loudnessRenderer.set(renderer.get<RendererPackFormatIdRefs>());
    }
    return loudnessRenderer;
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
    if (has<RendererPackFormatIdRefs>()) {
      renderer.set(get<RendererPackFormatIdRefs>());
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
    template class VectorParameter<RendererPackFormatIdRefs>;
    template class VectorParameter<RendererObjectIdRefs>;
  }  // namespace detail

}  // namespace adm
