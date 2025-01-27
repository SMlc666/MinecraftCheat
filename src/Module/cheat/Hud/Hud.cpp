#include "Hud.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <any>
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false},
                                                                     {"shortcut", false}};
cheat::Hud::Hud() : Module("Hud", MenuType::RENDER_MENU, ConfigData) {
}