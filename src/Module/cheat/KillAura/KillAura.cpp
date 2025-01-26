#include "KillAura.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"cps", 10}, {"range", 5.0f}, {"swing", false}};
static std::vector<Player *> PlayerList = {};
cheat::KillAura::KillAura() : Module("KillAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    bool enabled = module->getGUI().Get<bool>("enabled");
    if (!enabled) {
      return;
    }
    ClientInstance *mInstance = runtimes::getClientInstance();
    if (!mInstance) {
      return;
    }
    LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
    if (!mLocalPlayer) {
      return;
    }
    Dimension *mDimension = mLocalPlayer->mDimension;
    if (!mDimension) {
      return;
    }
    PlayerList.clear();
    mDimension->forEachPlayer([](Player &player) {
      PlayerList.push_back(&player);
      return false;
    });
    if (PlayerList.empty()) {
      return;
    }
    for (auto *player : PlayerList) {
    }
  });
}