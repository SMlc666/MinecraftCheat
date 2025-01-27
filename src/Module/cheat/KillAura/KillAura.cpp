#include "KillAura.hpp"

#include <math.h>
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <chrono>
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"cps", 10},        {"range", 5.0F},
    {"swing", false},   {"attackNum", 1},    {"antibot", false},
};
static std::vector<Player *> PlayerList = {};
static std::chrono::steady_clock::time_point LastAttackTime = std::chrono::steady_clock::now();
cheat::KillAura::KillAura() : Module("KillAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.SliderFloat("range", "范围", 0.0F, 10.0F);
    gui.SliderInt("cps", "攻击速度", 1, 20);
    gui.SliderInt("attackNum", "攻击数量", 1, 20);
    gui.CheckBox("swing", "挥手");
    gui.CheckBox("antibot", "反机器人");
  });
  setOnTick([](Module *module) {
    bool enabled = false;
    float Range = NAN;
    bool swing = false;
    int cps = 0;
    int attackNum = 0;
    bool antibot = false;
    try {
      enabled = module->getGUI().Get<bool>("enabled");
      Range = module->getGUI().Get<float>("range");
      swing = module->getGUI().Get<bool>("swing");
      cps = module->getGUI().Get<int>("cps");
      attackNum = module->getGUI().Get<int>("attackNum");
      antibot = module->getGUI().Get<bool>("antibot");
    } catch (const std::exception &e) {
      return;
    }
    int attackCount = 0;
    auto now = std::chrono::steady_clock::now();
    auto interval = std::chrono::milliseconds(1000 / std::max(cps, 1));
    if (!enabled) {
      return;
    }
    if (now - LastAttackTime < interval) {
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
      return true;
    });
    if (PlayerList.empty()) {
      return;
    }
    GameMode *mGameMode = &mLocalPlayer->getGameMode(); //LocalPlayer
    for (auto *player : PlayerList) {
      if (player == mLocalPlayer) {
        continue;
      }
      if (attackCount >= attackNum) {
        break;
      }
      float mDistance = mLocalPlayer->getDistance(player);
      if (mDistance <= Range) {
        attackCount++;
        mGameMode->attack(*player);
        if (swing) {
          mLocalPlayer->swing();
        }
      }
    }
    LastAttackTime = std::chrono::steady_clock::now();
  });
}