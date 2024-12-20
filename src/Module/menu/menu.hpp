#pragma once
#include <string>
#include <functional>
enum MenuType { MAIN_MENU, COMBAT_MENU, LOG_MENU, SCRIPT_MENU, CONFIG_MENU, DEBUG_MENU };
const std::unordered_map<MenuType, std::string> MenuNames = {
    {MAIN_MENU, "Main Menu"},     {COMBAT_MENU, "Combat Menu"}, {LOG_MENU, "Log Menu"},
    {SCRIPT_MENU, "Script Menu"}, {CONFIG_MENU, "Config Menu"}, {DEBUG_MENU, "Debug Menu"}};
extern const std::unordered_map<MenuType, std::function<void()>> MenuFunctions;