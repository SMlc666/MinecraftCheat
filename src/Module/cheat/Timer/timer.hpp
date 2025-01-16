#pragma once
#include "Module.hpp"
#include "gui/gui.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "game/minecraft/minecraft.hpp"
#include <unordered_map>
extern const std::unordered_map<std::string, std::any> TimerConfigData;
namespace cheat {
class Timer : public Module {
public:
  Timer() : Module("Timer", MenuType::COMBAT_MENU, TimerConfigData) {
    setOnEnable([](Module *module) {
      Minecraft *mc = runtimes::getMinecraftInstance();
      if (mc != nullptr) {
        mc->setSimTimeScale(module->getGUI().Get<float>("Scale"));
      }
    });
    setOnDisable([](Module *module) {
      Minecraft *mc = runtimes::getMinecraftInstance();
      if (mc != nullptr) {
        mc->setSimTimeScale(1.0F);
      }
    });
    setOnDraw([](Module *module) { module->getGUI().SliderFloat("Scale", "Scale", 1.0F, 10.0F); });
  }
};
} // namespace cheat