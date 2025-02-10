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
static MemTool::Hook attack_;
static Module *g_md;
bool GameMode_attack(GameMode *self, Actor *entity) {
  auto ret = attack_.call<bool>(self, entity);
  bool enabled = g_md->getGUI().Get<bool>("enabled");
  if (enabled) {
    bool swing = g_md->getGUI().Get<bool>("swing");
    int value = g_md->getGUI().Get<int>("value");
    auto *instance = runtimes::getClientInstance();
    if (instance != nullptr) {
      LocalPlayer *player = instance->getLocalPlayer();
      if (player != nullptr) {
        if (self->player == player) {
          for (int i = 0; i < value; i++) {
            attack_.call<void>(self, entity);
            if (swing) {
              player->swing();
            }
          }
        }
      }
    }
  }
  return ret;
}
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"swing", false}, {"value", 1}, {"shortcut", false}};
cheat::Seckill::Seckill() : Module("Seckill", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    g_md = module;
    auto *attack_func = getSign<void *>("GameMode::attack");
    attack_ = MemTool::Hook(attack_func, reinterpret_cast<void *>(GameMode_attack), nullptr, false);
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderInt("value", "连击次数", 0, 100);
    module->getGUI().CheckBox("swing", "挥手");
  });
}