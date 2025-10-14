#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "utility.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <raylib.h>
#include <rlgl.h>
#include <string>
#include <utility>
#include <vector>

struct Inspector {
public:
  void BeginGroup(const std::string &name) {
    if (activeGroup.has_value()) {
      TraceLog(LOG_ERROR,
               "Error: already in group '%s'. Cannot begin another.\n",
               *activeGroup->c_str());
      return;
    }
    activeGroup = name;
  }

  void EndGroup() {
    if (!activeGroup.has_value()) {
      TraceLog(LOG_ERROR, "Error: EndGroup() called with no active group.\n");
      return;
    }

    activeGroup.reset();
  }

  template <typename T>
  void AddItem(const std::string &name, T *value,
               std::function<void(float)> change,
               std::function<std::string()> str) {
    auto newItem = std::make_unique<Item<T>>(Item<T>{name, value, change, str});
    std::string groupName = activeGroup.value_or("default");

    for (auto &group : groups) {
      if (group.name == groupName) {
        group.items.push_back(std::move(newItem));
        return;
      }
    }

    groups.push_back(ItemGroup(groupName));
    groups.back().items.push_back(std::move(newItem));
  }

  void Next() {
    if (groups.empty())
      return;

    auto &group = groups[selectedGroup];

    if (group.items.empty())
      return;

    selectedItem = (SelectedItemIndex() + 1) % group.items.size();
  }

  void Prev() {
    if (groups.empty())
      return;

    auto &group = groups[selectedGroup];

    if (group.items.empty())
      return;

    size_t index = SelectedItemIndex();
    selectedItem = (index == 0) ? group.items.size() - 1 : index - 1;
  }

  void Change(float delta) {
    if (!groups.empty() && selectedGroup < groups.size()) {
      ItemGroup &group = groups[selectedGroup];
      if (!group.items.empty()) {
        size_t index = SelectedItemIndex();
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

    size_t index = SelectedItemIndex();

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
      if (!group.items.empty()) {
        return group.items[SelectedItemIndex()]->GetName();
      }
    }
    return "";
  }

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
    std::vector<std::unique_ptr<IItem>> items;

    explicit ItemGroup(std::string name) : name(std::move(name)) {}
  };

  size_t SelectedItemIndex() const {
    if (groups.empty() || selectedGroup >= groups.size())
      return 0;

    const auto &group = groups[selectedGroup];
    if (group.items.empty())
      return 0;

    return std::min(selectedItem, group.items.size() - 1);
  }

  std::vector<ItemGroup> groups;
  std::optional<std::string> activeGroup;

  size_t selectedGroup = 0;
  size_t selectedItem = 0;

  int fontSize = 32;
  int thickness = 4;
  Color textColor = WHITE;
  Color outlineColor = BLACK;
};

#endif // INSPECTOR_H
