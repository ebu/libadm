#include "adm/elements/authoring_information.hpp"
#include "adm/elements/loudness_renderer.hpp"

namespace adm {

  LoudnessRenderer Renderer::toLoudnessRenderer() const {
    return LoudnessRenderer::fromRenderer(*this);
  }

  void Renderer::print(std::ostream& os) const {
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

  void AuthoringInformation::print(std::ostream& os) const {
    os << "(referenceLayouts=" << get<ReferenceLayouts>().size()
       << ", renderers=" << get<Renderers>().size() << ")";
  }

  namespace detail {
    template class VectorParameter<RendererPackFormatIdRefs>;
    template class VectorParameter<ReferenceLayouts>;
    template class VectorParameter<Renderers>;
  }  // namespace detail

}  // namespace adm
