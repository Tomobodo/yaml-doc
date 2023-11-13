#include "yaml-doc/Doc.hpp"
#include "yaml-doc/exceptions/DocException.hpp"

#include <doctest/doctest.h>

struct InventoryItem
{
  std::string item;
  int quantity;
  int damage;
};

template<>
inline InventoryItem YAML::Doc::getValue<InventoryItem>()
{
  return { .item = getValue("item", "unknown"),
           .quantity = getValue<int>("quantity", 1),
           .damage = getValue<int>("damage", 0) };
}

TEST_SUITE("Testing YamlDoc")
{
  TEST_CASE("Parsing yaml file")
  {
    try {
      YAML::Doc doc = YAML::Doc::parseFile("tests_files/basic_parsing.yaml");
      YAML::Doc* nameDoc = doc.getNode("name");
      std::string name = nameDoc->getValue();

      CHECK(name == "Average Person");

      int age = doc.getNode("age")->getValue<int>();
      CHECK(age == 32);

      double weight = doc.getNode("weight")->getValue<double>();
      CHECK(weight == doctest::Approx(62.0));

      float height = doc.getNode("height")->getValue<float>();
      CHECK(height == doctest::Approx(1.68f));

      YAML::Doc* bananaNode = doc.getNode("inventory.0.item");
      CHECK(bananaNode != nullptr);

      std::string banana = bananaNode->getValue();
      CHECK(banana == "banana");

      int damage = doc.getNode("inventory.2.damage")->getValue<int>();
      CHECK(damage == 5);

      InventoryItem item = doc.getValue<InventoryItem>("inventory.2");
      CHECK(item.item == "dagger");

      float notFoundValue = 12.5f;
      doc.tryGetValue("non.existant.path", &notFoundValue);
      CHECK(notFoundValue == 12.5f);

      doc.tryGetValue<float>("non.existant.path", &notFoundValue, 51.3f);
      CHECK(notFoundValue == 51.3f);

    } catch (const YAML::DocException e) {
      FAIL(std::string(e.what()));
    }
  }
}
