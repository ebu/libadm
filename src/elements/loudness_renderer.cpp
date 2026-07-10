#include "adm/elements/loudness_renderer.hpp"

namespace adm {

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
