#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "utility.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <raylib.h>
#include <rlgl.h>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

struct Inspector {
public:
  template <typename T>
  void Add(const std::string &name, T *value, std::function<void(float)> change,
           std::function<std::string()> str) {
    std::type_index type = typeid(T);

    auto newItem = std::make_unique<Item<T>>(Item<T>{name, value, change, str});

    for (auto &group : groups) {
      if (group.type == type) {
        group.items.push_back(std::move(newItem));
        return;
      }
    }

    ItemGroup newGroup(type);
    newGroup.items.push_back(std::move(newItem));
    groups.push_back(std::move(newGroup));
  }

  void Next() {
    if (groups.empty())
      return;

    auto &group = groups[selectedGroup];

    if (group.items.empty())
      return;

    selectedItem = (selectedItem + 1) % group.items.size();
  }

  void Prev() {
    if (groups.empty())
      return;

    auto &group = groups[selectedGroup];

    if (group.items.empty())
      return;

    selectedItem =
        (selectedItem == 0) ? group.items.size() - 1 : selectedItem - 1;
  }

  void Change(float delta) {
    if (!groups.empty() && selectedGroup < groups.size()) {
      ItemGroup &group = groups[selectedGroup];
      if (group.items.size() > 0 && group.items.size() > 0) {
        size_t index = (selectedItem >= group.items.size())
                           ? group.items.size() - 1
                           : selectedItem;
        auto &item = group.items[index];
        item->Change(delta);
      }
    }
  }

  void ChangeGroup(int sign) {
    int s = (sign >= 0 ? 1 : -1);
    selectedGroup = (selectedGroup + groups.size() + s) % groups.size();
  }

  void Draw() {
    if (groups.empty() || selectedGroup >= groups.size())
      return;

    int x = 8;
    int y = 8;
    ItemGroup &group = groups[selectedGroup];
    DrawTextOutlined(group.name.c_str(), x, y, fontSize, textColor,
                     outlineColor, thickness);

    size_t index = (selectedItem >= group.items.size()) ? group.items.size() - 1
                                                        : selectedItem;

    for (size_t i = 0; i < group.items.size(); i++) {
      auto &item = group.items[i];
      y += fontSize + thickness;
      const char *text =
          TextFormat(" %c %s: %s", (i == index ? '>' : ' '),
                     item->GetName().c_str(), item->Str().c_str());
      DrawTextOutlined(text, x, y, fontSize, textColor, outlineColor,
                       thickness);
    }
  }

  std::string GetName() {
    if (!groups.empty() && selectedGroup < groups.size()) {
      ItemGroup &group = groups[selectedGroup];
      if (!group.items.empty() && selectedItem < group.items.size()) {
        return group.items[selectedItem]->GetName();
      }
    }
    return "";
  }

  size_t selectedGroup = 0;
  size_t selectedItem = 0;

private:
  struct IItem {
    virtual ~IItem() = default;

    virtual std::string GetName() const = 0;
    virtual std::string Str() const = 0;
    virtual void Change(float) = 0;
  };

  template <typename T> struct Item : IItem {
    std::string name;
    T *value;
    std::function<void(float)> change;
    std::function<std::string()> str;

    Item(std::string name, T *value, std::function<void(float)> change,
         std::function<std::string()> str)
        : name(std::move(name)), value(value), change(std::move(change)),
          str(std::move(str)) {}

    std::string GetName() const override { return name; }
    std::string Str() const override { return str(); }
    void Change(float value) override { return change(value); };
  };

  struct ItemGroup {
    std::string name;
    std::type_index type;
    std::vector<std::unique_ptr<IItem>> items;

    explicit ItemGroup(std::type_index t) : name(demangle(t)), type(t) {}
  };

  std::vector<ItemGroup> groups;

  int fontSize = 32;
  int thickness = 4;
  Color textColor = WHITE;
  Color outlineColor = BLACK;
};

#endif // INSPECTOR_H
