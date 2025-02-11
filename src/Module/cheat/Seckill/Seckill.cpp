#include "Seckill.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <unordered_map>

const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"swing", false}, {"value", 1}, {"shortcut", false}};
cheat::Seckill::Seckill() : Module("Seckill", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderInt("value", "连击次数", 0, 100);
    module->getGUI().CheckBox("swing", "挥手");
  });
  setOnAttack([](Module *module, MemTool::Hook *mem, Actor *entity) {
    try {
      int value = module->getGUI().Get<int>("value");
      bool swing = module->getGUI().Get<bool>("swing");
      auto *instance = runtimes::getClientInstance();
      if (!instance) {
        return;
      }
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player) {
        return;
      }
      for (int i = 0; i < value; i++) {
        mem->call<bool>(&player->getGameMode(), entity);
        if (swing) {
          player->swing();
        }
      }
    } catch (...) {
      return;
    }
  });
}