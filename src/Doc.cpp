#include "Doc.hpp"

#include "exceptions/DocFileException.hpp"
#include "exceptions/DocNodeException.hpp"
#include "exceptions/DocParserException.hpp"

#include <ostream>
#include <string>
#include <yaml.h>

namespace YAML {
Doc Doc::parseFile(const std::string& pPath)
{
  Doc doc;
  doc.name = "root";

  FILE* yamlFile = fopen(pPath.c_str(), "r");
  if (yamlFile == NULL) {
    throw DocFileException("Could not open file", pPath);
  }

  yaml_parser_t parser;
  yaml_event_t event;

  if (!yaml_parser_initialize(&parser)) {
    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    fclose(yamlFile);
    throw DocParserException("Could not initialize yaml parser");
  }

  yaml_parser_set_input_file(&parser, yamlFile);
  processYamlEvents(parser, event, doc);

  yaml_event_delete(&event);
  yaml_parser_delete(&parser);
  fclose(yamlFile);

  return std::move(doc);
}

Doc Doc::parseString(const std::string& pString)
{
  Doc doc;
  doc.name = "root";

  yaml_parser_t parser;
  yaml_event_t event;

  if (!yaml_parser_initialize(&parser)) {
    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    throw DocParserException("Could not initialize yaml parser");
  }

  yaml_parser_set_input_string(
    &parser, (unsigned const char*)pString.c_str(), (size_t)pString.length());
  processYamlEvents(parser, event, doc);

  yaml_event_delete(&event);
  yaml_parser_delete(&parser);

  return std::move(doc);
}

Doc::Doc() {}

Doc::Doc(Doc* pParent)
{
  parent = pParent;
}

Doc::Doc(const Doc& pOther)
{
  type = pOther.type;
  name = pOther.name;
  value = pOther.value;
  parent = pOther.parent;
  children = pOther.children;
}

Doc& Doc::operator=(const Doc& pOther)
{
  if (this == &pOther) {
    return *this;
  }

  type = pOther.type;
  name = pOther.name;
  value = pOther.value;
  parent = pOther.parent;
  children = pOther.children;

  return *this;
}

Doc::Doc(Doc&& pOther) noexcept
{
  type = std::move(pOther.type);
  name = std::move(pOther.name);
  value = std::move(pOther.value);
  parent = pOther.parent;
  children = std::move(pOther.children);

  pOther.parent = nullptr;
}

void Doc::processYamlEvents(yaml_parser_t& pParser,
                            yaml_event_t& pEvent,
                            Doc& pDoc)
{
  Doc* currentDoc = &pDoc;
  do {
    if (!yaml_parser_parse(&pParser, &pEvent)) {
      throw DocParserException(
        "An error occured while parsing the document : " +
        std::string(pParser.problem));
    }

    if (currentDoc == nullptr) {
      throw DocParserException("unexpected null node");
    }

    switch (pEvent.type) {
      case YAML_MAPPING_START_EVENT:
        if (currentDoc->type == SEQUENCE) {
          currentDoc = currentDoc->addSequenceItem();
        }
        currentDoc->type = MAPPING;
        break;

      case YAML_SEQUENCE_START_EVENT:
        if (currentDoc->type == SEQUENCE) {
          currentDoc = currentDoc->addSequenceItem();
        }
        currentDoc->type = SEQUENCE;
        break;

      case YAML_MAPPING_END_EVENT:
      case YAML_SEQUENCE_END_EVENT:
        if (currentDoc->parent) {
          currentDoc = currentDoc->parent;
        }
        break;

      case YAML_ALIAS_EVENT:
        break;

      case YAML_SCALAR_EVENT: {
        std::string scalarValue((char*)pEvent.data.scalar.value);
        if (currentDoc->type == SCALAR) {
          currentDoc->value = scalarValue;
          currentDoc = currentDoc->parent;
        } else if (currentDoc->type == SEQUENCE) {
          Doc* item = currentDoc->addSequenceItem();
          item->value = scalarValue;
        } else {
          currentDoc = currentDoc->createChild();
          currentDoc->name = scalarValue;
          currentDoc->type = SCALAR;
        }
      } break;
      default:
        break;
    }

    if (pEvent.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&pEvent);

  } while (pEvent.type != YAML_STREAM_END_EVENT);
}

Doc* Doc::addChild(Doc pChild)
{
  children.push_back(std::move(pChild));
  return &children.back();
}

Doc* Doc::createChild()
{
  children.emplace_back(this);
  return &children.back();
}

Doc* Doc::addSequenceItem()
{
  Doc* child = createChild();
  child->name = std::to_string(children.size() - 1);
  return child;
}

std::string Doc::toString(int pDepth) const
{
  std::string str = "";

  if (pDepth > 0) {
    for (int i = 0; i < pDepth; i++) {
      if (i == 0) {
        str += "|";
      } else {
        str += "--";
      }

      if (i == pDepth - 1) {
        str += " ";
      }
    }
  }

  str += name;

  if (children.size() == 0) {
    str += " -> " + value;
  }

  str += "\n";

  for (int i = 0; i < children.size(); i++) {
    str += children[i].toString(pDepth + 1);
  }

  return str;
}

std::string Doc::toString() const
{
  return toString(0);
}

Doc* Doc::getNode(const std::string& pPath)
{
  if (pPath.empty()) {
    return this;
  }

  std::stringstream ss(pPath);
  std::string token;
  Doc* currentDoc = this;

  while (std::getline(ss, token, '.')) {
    bool found = false;
    for (int i = 0; i < currentDoc->children.size(); i++) {
      if (currentDoc->children[i].name == token) {
        currentDoc = &currentDoc->children[i];
        found = true;
        break;
      }
    }

    if (!found) {
      currentDoc = nullptr;
      throw DocNodeException("Node " + pPath + " not found.");
    }
  }

  return currentDoc;
}

bool Doc::tryGetNode(const std::string& pPath, Doc*& pOut)
{
  if (pPath.empty()) {
    pOut = this;
    return true;
  }

  std::stringstream ss(pPath);
  std::string token;
  Doc* currentDoc = this;

  while (std::getline(ss, token, '.')) {
    bool found = false;
    for (int i = 0; i < currentDoc->children.size(); i++) {
      if (currentDoc->children[i].name == token) {
        currentDoc = &currentDoc->children[i];
        found = true;
        break;
      }
    }

    if (!found) {
      pOut = nullptr;
      return false;
    }
  }

  pOut = currentDoc;
  return true;
}

std::string Doc::getValue(const std::string& pPath)
{
  Doc* node = getNode(pPath);
  return node->getValue();
}

std::string Doc::getValue(const std::string& pPath, const std::string& pDefault)
{
  Doc* node;
  if (tryGetNode(pPath, node)) {
    return node->getValue();
  }
  return pDefault;
}

bool Doc::tryGetValue(const std::string& pPath,
                      std::string* pOut,
                      const std::string& pDefaultValue)
{
  Doc* node;
  if (tryGetNode(pPath, node)) {
    *pOut = node->getValue();
    return true;
  }

  *pOut = pDefaultValue;

  return false;
}

template<>
const char* Doc::getValueOr(const char* pDefault)
{
  if (children.size() == 0) {
    return value.c_str();
  }

  return pDefault;
}

std::ostream& operator<<(std::ostream& os, const Doc& doc)
{
  std::string str = doc.toString();
  os << str;
  return os;
}

}
