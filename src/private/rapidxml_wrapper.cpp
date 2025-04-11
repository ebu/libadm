#include "adm/private/rapidxml_wrapper.hpp"

namespace adm {
  namespace xml {

    // ---- XML DOCUMENT WRAPPER ---- //

    XmlNode XmlDocument::addNode(const std::string &name) {
      auto nameString = doc_.allocate_string(name.c_str());
      auto childNode = doc_.allocate_node(rapidxml::node_element, nameString);
      doc_.append_node(childNode);
      return XmlNode(childNode, discardDefaultValues_);
    }

    XmlNode XmlDocument::addDeclaration() {
      auto declaration = doc_.allocate_node(rapidxml::node_declaration);
      declaration->append_attribute(doc_.allocate_attribute("version", "1.0"));
      declaration->append_attribute(
          doc_.allocate_attribute("encoding", "utf-8"));
      doc_.append_node(declaration);
      return XmlNode(declaration, discardDefaultValues_);
    }

    XmlNode XmlDocument::addItuStructure() {
      auto ituAdmNode = addNode("ituADM");
      auto audioFormatExtendedNode = ituAdmNode.addNode("audioFormatExtended");
      return audioFormatExtendedNode;
    }

    XmlNode XmlDocument::addCoreMetadataAudioFormatExtended(
        XmlNode &parent) const {
      auto coreMetaDataNode = parent.addNode("coreMetadata");
      auto formatNode = coreMetaDataNode.addNode("format");
      auto audioFormatExtendedNode = formatNode.addNode("audioFormatExtended");
      return audioFormatExtendedNode;
    }

    XmlNode XmlDocument::addEbuStructure() {
      auto ebuCoreMainNode = addNode("ebuCoreMain");
      ebuCoreMainNode.addAttribute("xmlns:dc",
                                   "http://purl.org/dc/elements/1.1/");
      ebuCoreMainNode.addAttribute("xmlns",
                                   "urn:ebu:metadata-schema:ebuCore_2014");
      ebuCoreMainNode.addAttribute("xmlns:xsi",
                                   "http://www.w3.org/2001/XMLSchema-instance");
      ebuCoreMainNode.addAttribute("schema", "EBU_CORE_20140201.xsd");
      ebuCoreMainNode.addAttribute("xml:lang", "en");
      return addCoreMetadataAudioFormatExtended(ebuCoreMainNode);
    }

    // ---- XML NODE WRAPPER ---- //

    XmlNode::XmlNode(NodePtr node, bool discardDefaults)
        : node_(node), discardDefaultValues_(discardDefaults) {};

    void XmlNode::setValue(const std::string &value) {
      auto valueString = node_->document()->allocate_string(value.c_str());
      node_->value(valueString);
    }

    XmlNode XmlNode::addNode(const std::string &name) {
      auto nameString = node_->document()->allocate_string(name.c_str());
      auto childNode =
          node_->document()->allocate_node(rapidxml::node_element, nameString);
      node_->append_node(childNode);
      return XmlNode(childNode, discardDefaultValues_);
    }

    void XmlNode::addAttribute(const std::string &name,
                               const std::string &value) {
      auto nameString = node_->document()->allocate_string(name.c_str());
      auto valueString = node_->document()->allocate_string(value.c_str());
      auto attribute =
          node_->document()->allocate_attribute(nameString, valueString);
      node_->append_attribute(attribute);
    }

    void XmlNode::addElement(const std::string &name,
                             const std::string &value) {
      auto elementNode = addNode(name);
      elementNode.setValue(value);
    }

  }  // namespace xml
}  // namespace adm
