#pragma once
#include <string>
#include <functional>
enum MenuType {
  MAIN_MENU = 0,
  COMBAT_MENU = 1,
  LOG_MENU = 2,
  SCRIPT_MENU = 3,
  CONFIG_MENU = 4,
  DEBUG_MENU = 5,
  RENDER_MENU = 6
};
const std::unordered_map<MenuType, std::string> MenuNames = {
    {MAIN_MENU, "Main Menu"},     {COMBAT_MENU, "Combat Menu"}, {LOG_MENU, "Log Menu"},
    {SCRIPT_MENU, "Script Menu"}, {CONFIG_MENU, "Config Menu"}, {DEBUG_MENU, "Debug Menu"},
    {RENDER_MENU, "Render Menu"}};
extern const std::unordered_map<MenuType, std::function<void()>> MenuFunctions;