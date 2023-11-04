#pragma once

#include "exceptions/DocConversionException.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <yaml.h>

namespace YAML {

class Doc
{
public:
  enum Type
  {
    NONE,
    ROOT,
    SCALAR,
    MAPPING,
    SEQUENCE
  };

  static Doc parseFromFile(const std::string pPath);
  static Doc parseFromString(const std::string pString);

  Doc();
  Doc(Doc* pParent);
  Doc(const Doc& pOther);
  Doc& operator=(const Doc& pOther);
  Doc(Doc&& other) noexcept;

  Doc* addChild(Doc pChild);
  Doc* createChild();
  Doc* addSequenceItem();

  std::string toString() const;
  std::string toString(int pDepth) const;

  inline Doc::Type getType() const { return type; }

  inline std::string getName() const { return name; }

  inline std::vector<Doc> getChildren() const { return children; }

  inline Doc* getParent() const { return parent; }

  Doc* getNode(const std::string pPath);

  bool tryGetNode(const std::string pPath, Doc*& pOut);

  inline std::string getValue() { return value; }

  template<typename T>
  T getValue()
  {
    T returnValue;
    std::istringstream stream(value);
    stream >> returnValue;
    if (stream.fail() || !stream.eof()) {
      throw DocConversionException("Could not convert " + name + " to " +
                                   typeid(T).name());
    }

    return returnValue;
  }

  template<typename T>
  T getValueOr(T pDefault)
  {
    T returnValue;
    std::istringstream stream(value);
    stream >> returnValue;
    if (stream.fail() || !stream.eof()) {
      return pDefault;
    }

    return returnValue;
  }

  template<typename T>
  T getValue(std::string pPath)
  {
    Doc* node = getNode(pPath);
    return node->getValue<T>();
  }

  template<typename T>
  T getValue(std::string pPath, T pDefault)
  {
    Doc* node;
    if (tryGetNode(pPath, node)) {
      return node->getValueOr<T>(pDefault);
    }
    return pDefault;
  }

  std::string getValue(std::string pPath);

  std::string getValue(std::string pPath, std::string pDefault);

  template<typename T>
  bool tryGetValue(std::string pPath, T* pOut)
  {
    Doc* node;
    if (tryGetNode(pPath, node)) {
      *pOut = node->getValue<T>();
      return true;
    }
    return false;
  }

  template<typename T>
  bool tryGetValue(std::string pPath, T* pOut, T pDefaultValue)
  {
    Doc* node;
    if (tryGetNode(pPath, node)) {
      *pOut = node->getValue<T>();
      return true;
    }

    *pOut = pDefaultValue;
    return false;
  }

private:
  static void processYamlEvents(yaml_parser_t& pParser,
                                yaml_event_t& pEvent,
                                Doc& pDoc);

private:
  Type type = NONE;
  std::string name;
  std::string value;
  std::vector<Doc> children;
  Doc* parent = nullptr;
};

std::ostream& operator<<(std::ostream& os, const Doc& doc);

template<>
const char* Doc::getValueOr(const char* pDefault);

}
