#include "Timer.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/minecraft.hpp"
#include "game/minecraft/util/Timer/Timer.hpp"
#include <unordered_map>
static const uintptr_t TimerPatch = 0x1D99C60;
static const float TimerOrigin = 0.34526798129F; //1倍速
static const float TimerScale = 2.896301001511266F;
//NOLINTBEGIN
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"value", 1.0F},
};
//NOLINTEND
static const float NormalValue = 1.0F;
cheat::Timer::Timer() : Module("Timer", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    auto value = module->getGUI().Get<float>("value");
    MemTool::write(TimerPatch, value);
  });
  setOnDisable([](Module *module) { MemTool::write(TimerPatch, TimerOrigin); });
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("value", "value", 0.01, 10, [&module](float value) {
      if (module->getGUI().Get<bool>("enabled")) {
        MemTool::write(TimerPatch, value / TimerScale);
      }
    });
  });
  setOnLoad([](Module *module) {
    bool enabled;
    float value;
    try {
      enabled = module->getGUI().Get<bool>("enabled");
      value = module->getGUI().Get<float>("value");
    } catch (...) {
      return;
    }
    if (enabled) {
      MemTool::write(TimerPatch, value / TimerScale);
    }
  });
}