#include "Module.hpp"
#include "ModuleManager.hpp"
#include "gui/gui.hpp"
#include "menu/menu.hpp"
#include <unordered_map>
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"Scale", 1.0F},
};
class Timer : public Module {
  Timer() : Module("Timer", MenuType::COMBAT_MENU, ConfigData) {
    ModuleManager::addModule(this);
    setOnDraw([](Module *module, GUI gui) {

    });
  }
};