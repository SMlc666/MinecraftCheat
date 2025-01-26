#include "KillAura.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"cps", 10}, {"range", 5.0f}, {"swing", false}};
cheat::KillAura::KillAura() : Module("KillAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
}