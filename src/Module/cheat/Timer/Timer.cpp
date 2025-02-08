#include "Timer.hpp"
#include "MemTool.hpp"
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
    {"shortcut", false},
    {"value", 20.0F},
};
//NOLINTEND
static float NormalValue{};
cheat::Timer::Timer() : Module("Timer", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    try {
      float value = module->getGUI().Get<float>("value");
      ClientInstance *client = runtimes::getClientInstance();
      if (!client) {
        return;
      }
      auto *minecraft = client->minecraftPtr;
      if (!minecraft) {
        return;
      }
      NormalValue = minecraft->mRealTimer.getTimeScale();
      minecraft->mRealTimer.setTimeScale(value);
      minecraft->mSimTimer.setTimeScale(value);
    } catch (...) {
      return;
    }
  });
  setOnDisable([](Module *module) {
    try {
      ClientInstance *client = runtimes::getClientInstance();
      if (!client) {
        return;
      }
      auto *minecraft = client->minecraftPtr;
      if (!minecraft) {
        return;
      }
      if (NormalValue != 0) {
        minecraft->mRealTimer.setTimeScale(NormalValue);
        minecraft->mSimTimer.setTimeScale(NormalValue);
      }
    } catch (...) {
      return;
    }
  });
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("value", "value", 0.01F, 80.0F, [&module](float value) {
      try {
        if (module->getGUI().Get<bool>("enabled")) {
          ClientInstance *client = runtimes::getClientInstance();
          if (!client) {
            return;
          }
          auto *minecraft = client->minecraftPtr;
          if (!minecraft) {
            return;
          }
          minecraft->mRealTimer.setTimeScale(value);
          minecraft->mSimTimer.setTimeScale(value);
        }
      } catch (...) {
        return;
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
      ClientInstance *client = runtimes::getClientInstance();
      if (!client) {
        return;
      }
      auto *minecraft = client->minecraftPtr;
      if (!minecraft) {
        return;
      }
      minecraft->mRealTimer.setTimeScale(value);
      minecraft->mSimTimer.setTimeScale(value);
    }
  });
}