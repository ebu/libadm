#pragma once
#include "adm/document.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/private/rapidxml_formatter.hpp"

#include <iostream>
#include <string>

namespace adm {
  namespace xml {

    using NodePtr = rapidxml::xml_node<> *;

    class XmlNode;

    class XmlDocument {
     public:
      XmlDocument() = default;
      XmlNode addNode(const std::string &name);

      XmlNode addDeclaration();
      XmlNode addItuStructure();
      XmlNode addEbuStructure();

      void setDiscardDefaults(bool value) { discardDefaultValues_ = value; }

     private:
      friend std::ostream &operator<<(std::ostream &os, XmlDocument const &doc);

      rapidxml::xml_document<> doc_;
      bool discardDefaultValues_ = false;
    };

    class XmlNode {
     public:
      XmlNode() = default;
      XmlNode(NodePtr node, bool discardDefaults);

      // --- GENERAL ---- //
      XmlNode addNode(const std::string &name);
      void setValue(const std::string &value);

      template <typename ValueType>
      void setValue(const ValueType &value);

      // --- ATTRIBUTES ---- //
      void addAttribute(const std::string &name, const std::string &value);

      template <typename AttributeType, typename Source, typename Callable>
      void addAttribute(const Source &src, const std::string &key,
                        Callable formatter);
      template <typename AttributeType, typename Source>
      void addAttribute(const Source &src, const std::string &key);

      template <typename AttributeType, typename Source, typename Callable>
      void addOptionalAttribute(const Source &src, const std::string &key,
                                Callable formatter);
      template <typename AttributeType, typename Source>
      void addOptionalAttribute(const Source &src, const std::string &key);

      // --- ELEMENTS ---- //
      void addElement(const std::string &name, const std::string &value);

      template <typename ElementType, typename Source>
      void addElement(const Source &src, const std::string &name);

      template <typename ElementType, typename Callable>
      void addElement(const ElementType &admElement, const std::string &name,
                      Callable formatter);

      template <typename ElementType, typename Source>
      void addOptionalElement(const Source &src, const std::string &name);

      template <typename ElementType, typename Source, typename Callable>
      void addOptionalElement(const Source &src, const std::string &name,
                              Callable formatter);

      template <typename ElementType, typename Source, typename Callable>
      void addMultiElement(const Source &src, const std::string &name,
                           Callable formatter);

      template <typename ElementType, typename Source, typename Callable>
      void addOptionalMultiElement(const Source &src, const std::string &name,
                                   Callable formatter);

      template <typename ElementType, typename Source, typename Callable>
      void addElements(const Source &src, const std::string &name,
                       Callable formatter);

      template <typename ElementType, typename ElementTypeId, typename Source,
                typename Callable>
      void addBaseElements(const Source &src, const std::string &name,
                           Callable formatter);

      template <typename ElementType, typename AdmIdType, typename Source>
      void addReference(const Source &src, const std::string &name);

      template <typename ElementType, typename AdmIdType, typename Source>
      void addOptionalReference(const Source &src, const std::string &name);

      template <typename ElementType, typename AdmIdType, typename Source>
      void addReferences(const Source &src, const std::string &name);

      void addAudioTrackFormatReferences(
          const std::shared_ptr<const AudioStreamFormat> &src,
          const std::string &name);

     private:
      NodePtr node_;
      bool discardDefaultValues_ = true;
    };

    inline std::ostream &operator<<(std::ostream &os, XmlDocument const &doc) {
      return os << doc.doc_;
    }

    // ---- Implementation ---- //

    template <typename ValueType>
    void XmlNode::setValue(const ValueType &value) {
      setValue(detail::toString(value));
    }

    template <typename AttributeType, typename Source>
    void XmlNode::addAttribute(const Source &src, const std::string &key) {
      if (!(discardDefaultValues_ &&
            src->template isDefault<AttributeType>())) {
        auto value = src->template get<AttributeType>();
        addAttribute(key, detail::toString(value));
      }
    }

    template <typename AttributeType, typename Source, typename Callable>
    void XmlNode::addAttribute(const Source &src, const std::string &key,
                               Callable formatter) {
      if (!(discardDefaultValues_ &&
            src->template isDefault<AttributeType>())) {
        auto value = src->template get<AttributeType>();
        addAttribute(key, formatter(value));
      }
    }

    template <typename AttributeType, typename Source>
    void XmlNode::addOptionalAttribute(const Source &src,
                                       const std::string &key) {
      if (!(discardDefaultValues_ &&
            src->template isDefault<AttributeType>())) {
        if (src->template has<AttributeType>()) {
          auto value = src->template get<AttributeType>();
          addAttribute(key, detail::toString(value));
        }
      }
    }

    template <typename AttributeType, typename Source, typename Callable>
    void XmlNode::addOptionalAttribute(const Source &src,
                                       const std::string &key,
                                       Callable formatter) {
      if (!(discardDefaultValues_ &&
            src->template isDefault<AttributeType>())) {
        if (src->template has<AttributeType>()) {
          auto value = src->template get<AttributeType>();
          addAttribute(key, formatter(value));
        }
      }
    }

    template <typename ElementType, typename Source>
    void XmlNode::addElement(const Source &src, const std::string &name) {
      if (!(discardDefaultValues_ && src->template isDefault<ElementType>())) {
        auto value = src->template get<ElementType>();
        addElement(name, detail::toString(value));
      }
    }

    template <typename ElementType, typename Callable>
    void XmlNode::addElement(const ElementType &admElement,
                             const std::string &name, Callable formatter) {
      auto node = addNode(name);
      formatter(node, admElement);
    }

    template <typename ElementType, typename Source>
    void XmlNode::addOptionalElement(const Source &src,
                                     const std::string &name) {
      if (!(discardDefaultValues_ && src->template isDefault<ElementType>())) {
        if (src->template has<ElementType>()) {
          auto value = src->template get<ElementType>();
          addElement(name, detail::toString(value));
        }
      }
    }

    template <typename ElementType, typename Source, typename Callable>
    void XmlNode::addOptionalElement(const Source &src, const std::string &name,
                                     Callable formatter) {
      if (!(discardDefaultValues_ && src->template isDefault<ElementType>())) {
        if (src->template has<ElementType>()) {
          auto admElement = src->template get<ElementType>();
          auto node = addNode(name);
          formatter(node, admElement);
        }
      }
    }

    template <typename ElementType, typename Source, typename Callable>
    void XmlNode::addMultiElement(const Source &src, const std::string &name,
                                  Callable formatter) {
      if (!(discardDefaultValues_ && src->template isDefault<ElementType>())) {
        auto element = src->template get<ElementType>();
        formatter(*this, name, element);
      }
    }

    template <typename ElementType, typename Source, typename Callable>
    void XmlNode::addOptionalMultiElement(const Source &src,
                                          const std::string &name,
                                          Callable formatter) {
      if (!(discardDefaultValues_ && src->template isDefault<ElementType>())) {
        if (src->template has<ElementType>()) {
          auto element = src->template get<ElementType>();
          formatter(*this, name, element);
        }
      }
    }

    template <typename ElementType, typename Source, typename Callable>
    void XmlNode::addElements(const Source &src, const std::string &name,
                              Callable formatter) {
      for (auto &element : src->template getElements<ElementType>()) {
        addElement(element, name, formatter);
      }
    }

    template <typename ElementType, typename ElementTypeId, typename Source,
              typename Callable>
    void XmlNode::addBaseElements(const Source &src, const std::string &name,
                                  Callable formatter) {
      for (auto &element : src->template getElements<ElementType>()) {
        auto id = element->template get<ElementTypeId>();
        if (!isCommonDefinitionsId(id)) {
          addElement(element, name, formatter);
        }
      }
    }

    template <typename ElementType, typename AdmIdType, typename Source>
    void XmlNode::addReference(const Source &src, const std::string &name) {
      addElement<AdmIdType>(src->template getReference<ElementType>(), name);
    }

    template <typename ElementType, typename AdmIdType, typename Source>
    void XmlNode::addOptionalReference(const Source &src,
                                       const std::string &name) {
      auto referencedElement = src->template getReference<ElementType>();
      if (referencedElement) {
        addElement<AdmIdType>(referencedElement, name);
      }
    }

    template <typename ElementType, typename AdmIdType, typename Source>
    void XmlNode::addReferences(const Source &src, const std::string &name) {
      for (auto &element : src->template getReferences<ElementType>()) {
        addElement<AdmIdType>(element, name);
      }
    }

    inline void XmlNode::addAudioTrackFormatReferences(
        const std::shared_ptr<const AudioStreamFormat> &src,
        const std::string &name) {
      for (auto &welement : src->getAudioTrackFormatReferences()) {
        auto element = welement.lock();
        if (element) {
          addElement<AudioTrackFormatId>(element, name);
        }
      }
    }

  }  // namespace xml
}  // namespace adm
