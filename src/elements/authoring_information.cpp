#include "adm/elements/authoring_information.hpp"
#include "adm/elements/loudness_renderer.hpp"
#include "adm/document.hpp"

#include <algorithm>
#include <stdexcept>

namespace adm {

  void ReferenceLayout::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error(
          "ReferenceLayout already belongs to another Document");
    }

    if (newParent && packFormat_) {
      auto packFormatParent = packFormat_->getParent().lock();
      if (packFormatParent && packFormatParent != newParent) {
        throw std::runtime_error(
            "ReferenceLayout cannot refer to an AudioPackFormat in a "
            "different document");
      }
      if (!packFormatParent) {
        newParent->add(packFormat_);
      }
    }

    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& ReferenceLayout::getParent() const {
    return parent_;
  }

  bool AuthoringRenderer::addReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    auto rendererParent = parent_.lock();
    auto packFormatParent = packFormat->getParent().lock();
    if (rendererParent) {
      if (packFormatParent && packFormatParent != rendererParent) {
        throw std::runtime_error(
            "AuthoringRenderer cannot refer to an AudioPackFormat in a "
            "different document");
      }
      if (!packFormatParent) {
        rendererParent->add(packFormat);
      }
    }

    auto it = std::find(packFormats_.begin(), packFormats_.end(), packFormat);
    if (it == packFormats_.end()) {
      packFormats_.push_back(std::move(packFormat));
      return true;
    }
    return false;
  }

  void AuthoringRenderer::removeReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    auto it = std::find(packFormats_.begin(), packFormats_.end(), packFormat);
    if (it != packFormats_.end()) {
      packFormats_.erase(it);
    }
  }

  ElementRange<AudioPackFormat> AuthoringRenderer::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return detail::makeElementRange<AudioPackFormat>(packFormats_);
  }

  ElementRange<const AudioPackFormat> AuthoringRenderer::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return detail::makeElementRange<AudioPackFormat>(packFormats_);
  }

  void AuthoringRenderer::clearReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    packFormats_.clear();
  }

  void AuthoringRenderer::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error(
          "AuthoringRenderer already belongs to another Document");
    }

    if (newParent) {
      for (auto const& packFormat : packFormats_) {
        auto packFormatParent = packFormat->getParent().lock();
        if (packFormatParent && packFormatParent != newParent) {
          throw std::runtime_error(
              "AuthoringRenderer cannot refer to an AudioPackFormat in a "
              "different document");
        }
      }
      for (auto const& packFormat : packFormats_) {
        if (!packFormat->getParent().lock()) {
          newParent->add(packFormat);
        }
      }
    }

    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& AuthoringRenderer::getParent() const {
    return parent_;
  }

  LoudnessRenderer AuthoringRenderer::toLoudnessRenderer() const {
    return LoudnessRenderer::fromRenderer(*this);
  }

  void AuthoringRenderer::print(std::ostream& os) const {
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

  bool AuthoringInformation::add(ReferenceLayout layout) {
    auto layouts =
        has<ReferenceLayouts>() ? get<ReferenceLayouts>() : ReferenceLayouts{};
    if (std::find(layouts.begin(), layouts.end(), layout) == layouts.end()) {
      if (auto parent = parent_.lock()) {
        layout.setParent(parent);
      }
      layouts.push_back(std::move(layout));
      set(std::move(layouts));
      return true;
    }
    return false;
  }

  void AuthoringInformation::set(ReferenceLayouts layouts) {
    if (auto parent = parent_.lock()) {
      for (auto& layout : layouts) {
        layout.setParent(parent);
      }
    }
    detail::AuthoringInformationBase::set(std::move(layouts));
  }

  bool AuthoringInformation::add(AuthoringRenderer renderer) {
    auto renderers = has<Renderers>() ? get<Renderers>() : Renderers{};
    if (std::find(renderers.begin(), renderers.end(), renderer) ==
        renderers.end()) {
      if (auto parent = parent_.lock()) {
        renderer.setParent(parent);
      }
      renderers.push_back(std::move(renderer));
      set(std::move(renderers));
      return true;
    }
    return false;
  }

  void AuthoringInformation::set(Renderers renderers) {
    if (auto parent = parent_.lock()) {
      for (auto& renderer : renderers) {
        renderer.setParent(parent);
      }
    }
    detail::AuthoringInformationBase::set(std::move(renderers));
  }

  void AuthoringInformation::setParent(std::weak_ptr<Document> document) {
    auto currentParent = parent_.lock();
    auto newParent = document.lock();
    if (currentParent && newParent && currentParent != newParent) {
      throw std::runtime_error(
          "AuthoringInformation already belongs to another Document");
    }

    auto layouts =
        has<ReferenceLayouts>() ? get<ReferenceLayouts>() : ReferenceLayouts{};
    auto renderers = has<Renderers>() ? get<Renderers>() : Renderers{};
    for (auto& layout : layouts) {
      layout.setParent(document);
    }
    for (auto& renderer : renderers) {
      renderer.setParent(document);
    }
    detail::AuthoringInformationBase::set(std::move(layouts));
    detail::AuthoringInformationBase::set(std::move(renderers));
    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& AuthoringInformation::getParent() const {
    return parent_;
  }

  void AuthoringInformation::print(std::ostream& os) const {
    os << "(referenceLayouts=" << get<ReferenceLayouts>().size()
       << ", renderers=" << get<Renderers>().size() << ")";
  }
  namespace detail {
    template class VectorParameter<ReferenceLayouts>;
    template class VectorParameter<Renderers>;
  }  // namespace detail

}  // namespace adm
