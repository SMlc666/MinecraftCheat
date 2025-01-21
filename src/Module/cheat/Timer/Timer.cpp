#include "Timer.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/minecraft.hpp"
#include "game/minecraft/util/Timer/Timer.hpp"
#include <unordered_map>
//NOLINTBEGIN
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"value", 1.0F},
};
//NOLINTEND
static const float NormalValue = 1.0F;
cheat::Timer::Timer() : Module("Timer", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    auto *client = runtimes::getClientInstance();
    if (client != nullptr) {
      client->minecraftPtr->getRealTimer().setTimeScale(module->getGUI().Get<float>("value"));
    }
  });
  setOnDisable([](Module *module) {
    auto *client = runtimes::getClientInstance();
    if (client != nullptr) {
      client->minecraftPtr->getRealTimer().setTimeScale(NormalValue);
    }
  });
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("value", "value", 0.01, 10, [&module](float value) {
      if (module->getGUI().Get<bool>("enabled")) {
        auto *client = runtimes::getClientInstance();
        if (client != nullptr) {
          client->minecraftPtr->getRealTimer().setTimeScale(value);
        }
      }
    });
  });
}