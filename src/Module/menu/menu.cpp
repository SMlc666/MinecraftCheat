#include "menu.hpp"
#include "menu/menus/Combat.hpp"
#include "menu/menus/Config.hpp"
#include "menu/menus/Debug.hpp"
#include "menu/menus/Log.hpp"
#include "menu/menus/Main.hpp"
#include "menu/menus/Render.hpp"
#include "menu/menus/Script.hpp"
#include <cstdlib>
#include <unwind.h>

const std::unordered_map<MenuType, std::function<void()>> MenuFunctions = {
    {MenuType::MAIN_MENU, drawMainMenu},     {MenuType::COMBAT_MENU, drawCombatMenu},
    {MenuType::LOG_MENU, drawLogMenu},       {MenuType::SCRIPT_MENU, drawScriptMenu},
    {MenuType::CONFIG_MENU, drawConfigMenu}, {MenuType::DEBUG_MENU, drawDebugMenu},
    {MenuType::RENDER_MENU, drawRenderMenu}};