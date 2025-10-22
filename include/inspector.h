#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <algorithm>
#include <cstddef>
#include <functional>
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
               activeGroup->c_str());
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

  void RemoveGroup(const std::string &name) {
    for (auto it = groups.begin(); it != groups.end();) {
      if (it->name == name) {
        it = groups.erase(it);
      } else {
        ++it;
      }
    }
  }

  void AddItem(const std::string &name, std::function<void(double)> change,
               std::function<std::string()> str) {
    auto newItem = Item{name, change, str};
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

  void RemoveItem(const std::string &groupName, const std::string &itemName) {
    for (auto &group : groups) {
      if (group.name == groupName) {
        for (auto it = group.items.begin(); it != group.items.end();) {
          if (it->name == itemName) {
            it = group.items.erase(it);
          } else {
            ++it;
          }
        }
        break;
      }
    }
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

  void Change(double delta) {
    if (groups.empty() || selectedGroup >= groups.size())
      return;
    ItemGroup &group = groups[selectedGroup];
    if (group.items.empty())
      return;
    size_t index = SelectedItemIndex();
    auto &item = group.items[index];
    item.change(delta);
  }

  void ChangeGroup(int sign) {
    if (groups.empty())
      return;
    int s = (sign >= 0 ? 1 : -1);
    selectedGroup = (selectedGroup + groups.size() + s) % groups.size();
  }

  void Draw() {
    if (groups.empty() || selectedGroup >= groups.size())
      return;

    int x = 8;
    int y = 8;
    ItemGroup &group = groups[selectedGroup];
    DrawText(group.name.c_str(), x, y, fontSize, textColor);

    size_t index = SelectedItemIndex();

    for (size_t i = 0; i < group.items.size(); i++) {
      auto &item = group.items[i];
      y += fontSize;
      const char *text = TextFormat(" %c %s: %s", (i == index ? '>' : ' '),
                                    item.name.c_str(), item.str().c_str());
      DrawText(text, x, y, fontSize, textColor);
    }
  }

  std::string GetItemName() {
    if (!groups.empty() && selectedGroup < groups.size()) {
      ItemGroup &group = groups[selectedGroup];
      if (!group.items.empty()) {
        return group.items[SelectedItemIndex()].name;
      }
    }
    return "";
  }

  std::string GetGroupName() {
    if (!groups.empty() && selectedGroup < groups.size()) {
      return groups[selectedGroup].name;
    }
    return "";
  }

private:
  struct Item {
    std::string name;
    std::function<void(double)> change;
    std::function<std::string()> str;

    Item(std::string name, std::function<void(double)> change,
         std::function<std::string()> str)
        : name(std::move(name)), change(std::move(change)),
          str(std::move(str)) {}
  };

  struct ItemGroup {
    std::string name;
    std::vector<Item> items;

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
  Color textColor = WHITE;
};

#endif // INSPECTOR_H
