#pragma once
#include <boost/optional.hpp>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "adm/document.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements/type_descriptor.hpp"
#include "adm/private/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"

namespace adm {
  namespace xml {
    using NodePtr = rapidxml::xml_node<>*;
    /// @brief Implementation details
    namespace detail {

      /**
       * Helper function to invoke Target::set on both smart pointers and
       * references in a uniform way, removing the need for multiple
       * overloadeds of the caller.
       */
      template <typename T, typename... Args>
      void invokeSet(std::shared_ptr<T> target, Args&&... args) {
        target->set(std::forward<Args>(args)...);
      }

      /**
       * Helper function to invoke Target::set on both smart pointers and
       * references in a uniform way, removing the need for multiple
       * overloadeds of the caller.
       */
      template <typename T, typename... Args>
      void invokeSet(T& target, Args&&... args) {
        target.set(std::forward<Args>(args)...);
      }

      /**
       * Helper function to invoke Target::add on both smart pointers and
       * references in a uniform way, removing the need for multiple
       * overloadeds of the caller.
       */
      template <typename T, typename... Args>
      void invokeAdd(T& target, Args&&... args) {
        target.add(std::forward<Args>(args)...);
      }

      /**
       * Helper function to invoke Target::add on both smart pointers and
       * references in a uniform way, removing the need for multiple
       * overloadeds of the caller.
       */
      template <typename T, typename... Args>
      void invokeAdd(std::shared_ptr<T> target, Args&&... args) {
        target->add(std::forward<Args>(args)...);
      }

      /**
       * @brief Find the first element within a node with a specified name
       *
       * @returns NodePtr to first element or a nullptr if no element could
       * not be found.
       */
      NodePtr findElement(NodePtr node, const std::string& name) {
        for (NodePtr elementNode = node->first_node(); elementNode;
             elementNode = elementNode->next_sibling()) {
          if (std::string(elementNode->name()) == name) {
            return elementNode;
          }
        }
        return nullptr;
      }

      /**
       * @brief Find all elements within a node with a specified name
       *
       * @returns a vector of NodePtr or an empty vector if no element could
       * not be found.
       */
      std::vector<NodePtr> findElements(NodePtr node, const std::string& name) {
        std::vector<NodePtr> elements;
        for (NodePtr elementNode = node->first_node(); elementNode;
             elementNode = elementNode->next_sibling()) {
          if (std::string(elementNode->name()) == name) {
            elements.push_back(elementNode);
          }
        }
        return elements;
      }

      template <typename T>
      struct TypeTag {};

      inline int parseImpl(const std::string& v, TypeTag<int>) {
        return std::stoi(v);
      }
      inline unsigned int parseImpl(const std::string& v,
                                    TypeTag<unsigned int>) {
        return std::stoul(v);
      }
      inline std::string parseImpl(const std::string& v, TypeTag<std::string>) {
        return v;
      }
      inline float parseImpl(const std::string& v, TypeTag<float>) {
        return std::stof(v);
      }
      inline double parseImpl(const std::string& v, TypeTag<double>) {
        return std::stod(v);
      }
      inline bool parseImpl(const std::string& v, TypeTag<bool>) {
        return std::stoi(v) != 0;
      }

      template <typename NT>
      NT parseDefault(const std::string& v) {
        typedef typename NT::value_type value_type;
        typedef TypeTag<value_type> DispatchTypeTag;
        return NT(parseImpl(v, DispatchTypeTag()));
      }
    }  // namespace detail

    // ---- attributes ---- //
    template <typename NT, typename Callable>
    boost::optional<NT> parseOptionalAttribute(NodePtr node,
                                               const char* attributeName,
                                               Callable parser) {
      auto attribute = node->first_attribute(attributeName);
      boost::optional<NT> value;
      if (attribute) {
        value = NT(parser(attribute->value()));
        /*
        try {
          value = NT(parser(attribute->value()));
        } catch (...) {
          errorHandler(FATAL_ERROR, e.what());
          return boost::none;
        }
        */
      }
      return value;
    }

    template <typename NT>
    boost::optional<NT> parseOptionalAttribute(NodePtr node,
                                               const char* attributeName) {
      return parseOptionalAttribute<NT>(node, attributeName,
                                        detail::parseDefault<NT>);
    }

    template <typename NT, typename Callable>
    NT parseAttribute(NodePtr node, const char* attributeName,
                      Callable parser) {
      auto value = parseOptionalAttribute<NT>(node, attributeName, parser);
      if (value != boost::none) {
        return value.get();
      } else {
        std::stringstream errorString;
        int errorLine = getDocumentLine(node);
        errorString << "mandatory attribute '" << attributeName
                    << "' not found in " << node->name() << " (" << errorLine
                    << ")";
        throw std::runtime_error(errorString.str());
      }
    }

    template <typename NT>
    NT parseAttribute(NodePtr node, const char* attributeName) {
      return parseAttribute<NT>(node, attributeName, detail::parseDefault<NT>);
    }

    template <typename NT, typename Target, typename Callable>
    void setOptionalAttribute(NodePtr node, const char* attributeName,
                              Target& target, Callable parser) {
      auto value = parseOptionalAttribute<NT>(node, attributeName, parser);
      if (value != boost::none) {
        detail::invokeSet(target, value.get());
      }
    }

    template <typename NT, typename Target>
    void setOptionalAttribute(NodePtr node, const char* attributeName,
                              Target& target) {
      setOptionalAttribute<NT>(node, attributeName, target,
                               detail::parseDefault<NT>);
    }

    // ---- element ---- //
    template <typename NT, typename Callable>
    boost::optional<NT> parseOptionalElement(NodePtr node,
                                             const char* elementName,
                                             Callable parser) {
      auto element = detail::findElement(node, elementName);
      boost::optional<NT> value;
      if (element) {
        value = NT(parser(element));
      }
      return value;
    }

    template <typename NT, typename Target, typename Callable>
    NT parseElement(NodePtr node, const char* elementName, Callable parser) {
      auto value = parseOptionalElement<NT>(node, elementName, parser);
      if (value != boost::none) {
        return value.get();
      } else {
        std::stringstream errorString;
        int errorLine = getDocumentLine(node);
        errorString << "mandatory element '" << elementName << "' not found ("
                    << errorLine << ")";
        throw std::runtime_error(errorString.str());
      }
    }

    template <typename NT, typename Target, typename Callable>
    void addOptionalElement(NodePtr node, const char* elementName,
                            Target& target, Callable parser) {
      auto value = parseOptionalElement<NT>(node, elementName, parser);
      if (value != boost::none) {
        detail::invokeAdd(target, value.get());
      }
    }

    template <typename NT, typename Target, typename Callable>
    void addOptionalElements(NodePtr node, const char* elementName,
                             Target& target, Callable parser) {
      auto elements = detail::findElements(node, elementName);
      for (auto& element : elements) {
        detail::invokeAdd(target, NT(parser(element)));
      }
    }

    template <typename NT, typename Target, typename Callable>
    void setOptionalElement(NodePtr node, const char* elementName,
                            Target& target, Callable parser) {
      auto value = parseOptionalElement<NT>(node, elementName, parser);
      if (value != boost::none) {
        detail::invokeSet(target, value.get());
      }
    }

    template <typename NT, typename Target>
    void setOptionalElement(NodePtr node, const char* elementName,
                            Target& target) {
      auto element = detail::findElement(node, elementName);
      if (element) {
        detail::invokeSet(target, detail::parseDefault<NT>(element->value()));
      }
    }

    template <typename NT, typename Callable>
    boost::optional<NT> parseOptionalMultiElement(NodePtr node,
                                                  const char* elementName,
                                                  Callable parser) {
      auto elements = detail::findElements(node, elementName);
      boost::optional<NT> value;
      if (elements.size() > 0) {
        value = NT(parser(elements));
      }
      return value;
    }

    template <typename NT, typename Target, typename Callable>
    void setOptionalMultiElement(NodePtr node, const char* elementName,
                                 Target& target, Callable parser) {
      auto value = parseOptionalMultiElement<NT>(node, elementName, parser);
      if (value != boost::none) {
        detail::invokeSet(target, value.get());
      }
    }

    template <typename NT, typename Target, typename Callable>
    void setMultiElement(NodePtr node, const char* elementName, Target& target,
                         Callable parser) {
      auto value = parseOptionalMultiElement<NT>(node, elementName, parser);
      if (value != boost::none) {
        detail::invokeSet(target, value.get());
      } else {
        std::stringstream errorString;
        int errorLine = getDocumentLine(node);
        errorString << "mandatory element '" << elementName << "' not found"
                    << errorLine << ")";
        ;
        throw std::runtime_error(errorString.str());
      }
    }

    // ---- references ---- //
    template <typename NT, typename Src, typename Target, typename Callable>
    void addOptionalReferences(NodePtr node, const char* elementName,
                               const Src src, Target& target, Callable parser) {
      auto elements = detail::findElements(node, elementName);
      for (auto& elementNode : elements) {
        target[src].push_back(NT(parser(elementNode->value())));
      }
    }

    template <typename NT, typename Src, typename Target, typename Callable>
    void setOptionalReference(NodePtr node, const char* elementName,
                              const Src src, Target& target, Callable parser) {
      auto elementNode = detail::findElement(node, elementName);
      if (elementNode) {
        target[src] = NT(parser(elementNode->value()));
      }
    }

    // ---- value ---- //
    template <typename NT, typename Callable>
    NT parseValue(NodePtr node, Callable parser) {
      auto value = node->value();
      if (value) {
        return parser(value);
      } else {
        std::stringstream errorString;
        errorString << "element does not have a value";
        throw std::runtime_error(errorString.str());
      }
    }

    template <typename NT, typename Target, typename Callable>
    void setValue(NodePtr node, Target& target, Callable parser) {
      detail::invokeSet(target, parseValue<NT>(node, parser));
    }

    template <typename NT, typename Target>
    void setValue(NodePtr node, Target& target) {
      detail::invokeSet(target, parseValue<NT>(node, detail::parseDefault<NT>));
    }

    // ---- sanity checks ---- //
    template <typename NT>
    void checkChannelType(NT id, boost::optional<TypeDescriptor> typeLabel,
                          boost::optional<TypeDescriptor> typeDefinition) {
      if (typeDefinition != boost::none &&
          id.template get<TypeDescriptor>() != typeDefinition.get()) {
        throw std::runtime_error("TypeDefinition and id do not match");
      }
      if (typeLabel != boost::none &&
          id.template get<TypeDescriptor>() != typeLabel.get()) {
        throw std::runtime_error("TypeLabel and id do not match");
      }
    }

    FormatDescriptor checkFormat(
        boost::optional<FormatDescriptor> formatLabel,
        boost::optional<FormatDescriptor> formatDefinition) {
      if (formatLabel != boost::none && formatDefinition != boost::none) {
        if (formatLabel.get() != formatDefinition.get()) {
          throw std::runtime_error(
              "formatLabel and formatDefinition do not match");
        }
        return formatLabel.get();
      }
      if (formatLabel != boost::none) {
        return formatLabel.get();
      }
      if (formatDefinition != boost::none) {
        return formatDefinition.get();
      }
      throw std::runtime_error(
          "Neither FormatLabel nor formatDefinition found");
    }

  }  // namespace xml
}  // namespace adm
