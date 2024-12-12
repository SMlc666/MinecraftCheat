#pragma once
#include <string>
#include <functional>
#include <map>
enum MenuType {
  MAIN_MENU,
  COMBAT_MENU,
  LOG_MENU,
};
const std::unordered_map<MenuType, std::string> MenuNames = {{MAIN_MENU, "Main Menu"},
                                                             {COMBAT_MENU, "Combat Menu"}};
extern const std::unordered_map<MenuType, std::function<void()>> MenuFunctions;