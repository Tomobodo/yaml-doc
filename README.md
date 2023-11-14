# yaml-doc

Simple Yaml Document lib

## Building

### installing Dependencies

you'll need [cmake](https://cmake.org/) and a c++ compiler (preferably gcc or clang, not tested with others yet)

yaml-doc also depends on [libyaml](https://github.com/yaml/libyaml)

libyaml is added as a git submodule so clone the project using --recursive option and every dependencies should be pulled

### Cloning

```shell
git clone git@github.com:tomobodo/yaml-doc.git --recursive
```

### Building the library

```shell
cd yaml-doc
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja # Ninja optional
cmake --build build
```

the output lib will be in build/Release/lib/libyaml-doc.a

## Testing

### Building the tests

in yaml-doc project folder:

```shell
cmake -B build 
cd build
cmake --build . --target yaml-doc yaml-doc-tests
ctest
```

## Usage

### linking

add the include directory to your include path
and the library to your linker

you can also just add the library as a subdirectory with cmake

```cmake
add_subdirectory(path/to/yaml-doc yaml-doc)
```

The easiest way to use yaml-doc for now is to add the repo as a gitmodule and add the library as a subdirectory

```shell
git submodule add git@github.com:tomobodo/yaml-doc.git libs/yaml-doc
```

then add the subdirectory to your cmake project

```cmake
add_subdirectory(libs/yaml-doc yaml-doc)
```

don't forget to link the library to your target

```cmake
target_link_libraries(${YOUR_TARGET} PRIVATE yaml-doc)
```

### Examples

#### parsing a yaml string

```shell
cmake --build build --target yaml-doc-example
```

```cpp
// main.cpp
#include <iostream>
#include <string>

#include "yaml-doc/Doc.hpp"

const std::string exampleDoc =
  R"YAML(name: John Doe
age: 42
relationships:
  - name: Ralfgor the Destroyer
    relationship: could be better not gonna lie
  - name: Obliterion the Unmaker
    relationship: it's complicated
  - name: Alice the mind warper
    relationship: she's the one
equipment:
  sword:
    name: the cheesy slicer
    damage: -10
  shield:
    name: the rusty shield
    defense: 5
inventory:
  - name: sword
    damage: 10
  - name: shield
    defense: 5
  - name: potion
    effect: heal
    value: 10.4
    count: 3
)YAML";

struct InventoryItem
{
  std::string name;
  int damage = 0;
  int defense = 0;
  std::string effect = "none";
  float value = 0.0f;
  int count = 1;
};

// you can write your own conversion functions for your own types by
// specializing the YAML::Doc::getValue<T>() template "this" is a YAML::Doc*
template<>
InventoryItem YAML::Doc::getValue<InventoryItem>()
{
  InventoryItem item;
  item.name = getValue("name");
  item.damage = getValue<int>("damage", 0);
  item.defense = getValue<int>("defense", 0);
  item.effect = getValue("effect", "none");
  item.value = getValue<float>("value", 0.0f);
  item.count = getValue<int>("count", 1);
  return item;
}

int main(int argc, char** argv)
{
  try { // yaml-doc can throws several exceptions based on the nature of the
        // error, see includes/yaml-doc/exceptions.h
        // YAML::Doc doc = YAML::Doc::parseFile("character.yaml");
        // this will throw a DocFileException if the file doesn't exist or if it
    // can't be opened this will throw a DocParserException if the file is not a
    // valid yaml file you can also parse a string using
    // YAML::Doc::parseString(const std::string &pString)
    YAML::Doc doc = YAML::Doc::parseString(exampleDoc);

    // once parsed the yaml document is a tree where each property is a node,
    // leaf have values, branches have children you can access nodes using the
    // getNode("node/path") method
    YAML::Doc* nameNode = doc.getNode("name");
    // this will throw a DocNodeException if the node doesn't exist

    // you can access the name of a node using the getName() method
    std::cout << "This node is named " << nameNode->getName()
              << std::endl; // This node is named name

    // you can access the type of a node using the getType() method, it return
    // Doc::Type enum wich can be either Doc::Type::NONE, Doc::Type::ROOT,
    // Doc::Type::SCALAR, Doc::Type::SEQUENCE or Doc::Type::MAPPING

    // you can access the value of a node using the getValue() method
    std::cout << "This is " << nameNode->getValue()
              << std::endl; // This is John Doe

    // you can also access the nodes values using the path directly
    std::cout << "Our hero has equiped " << doc.getValue("equipment.sword.name")
              << std::endl; // Our hero has equiped the cheesy slicer

    // this will throw a DocNodeException if the node doesn't exist, you can
    // either catch the exception or you can also use tryGetNode or tryGetValue
    // which will return true if the node exists and take a pointer to a
    // variable to store the value. If the node doesn't exist the variable will
    // be left untouched and no exception will be thrown. tryGetValue can also
    // take a default value as a second argument

    YAML::Doc* shieldNode;
    if (doc.tryGetNode("equipment.shield", shieldNode)) {
      std::cout << "Our hero has equiped " << shieldNode->getValue("name")
                << std::endl; // Our hero has equiped the rusty shield
    }

    std::string lukeRelationShip;
    doc.tryGetValue("relationship.3.relationship",
                    &lukeRelationShip,
                    "terrible"); // this path doesn't exist so lukeRelationShip
                                 // will be set to "terrible"

    std::cout << "The hero's relationship with Luke is " << lukeRelationShip
              << std::endl; // The hero's relationship with Luke is terrible

    // you can also iterate over the children of a node
    YAML::Doc* inventoryNode = doc.getNode("inventory");
    for (auto& itemNode : inventoryNode->getChildren()) {
      std::cout << "Our hero has " << itemNode.getValue<int>("count", 1) << " "
                << itemNode.getValue("name") << std::endl;
    }

    // as seen above you can use generic getValue<T>() method to get a value as
    // a specific type this will throw a DocConversionException if the value
    // can't be converted to the requested type you can write your own
    // conversion functions for your own types by specializing the
    // YAML::Doc::getValue<T>() template

    for (auto& itemNode : inventoryNode->getChildren()) {
      InventoryItem item = itemNode.getValue<InventoryItem>();
      std::cout << item.name << std::endl;
      std::cout << item.value << std::endl;
    }

  } catch (const YAML::DocException& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
```
