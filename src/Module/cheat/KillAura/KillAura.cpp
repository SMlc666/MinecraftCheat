#include "KillAura.hpp"
#include <cmath>
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
#include <random>
static const std::vector<std::string> PriorityItems = {"Health", "Distance"};
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"cps", 10},     {"range", 5.0F},    {"swing", false},
    {"attackNum", 1},   {"antibot", false},  {"fov", 180.0F}, {"failurerate", 0}, {"priority", 0}};
static std::vector<Player *> PlayerList = {};
static std::chrono::steady_clock::time_point LastAttackTime = std::chrono::steady_clock::now();
static std::random_device g_rd;
static std::uniform_int_distribution<> g_dist(0, 100);
static std::mt19937 g_gen(g_rd());
static bool isInFov(LocalPlayer *mLocalPlayer, Player *target, float maxFov) {
  if (maxFov >= 360.0F) {
    return true;
  }
  glm::vec3 localPos = mLocalPlayer->getPosition();
  glm::vec3 targetPos = target->getPosition();
  glm::vec3 direction = glm::normalize(glm::vec3(targetPos.x - localPos.x,
                                                 0, // 忽略Y轴差异
                                                 targetPos.z - localPos.z));
  float targetYaw = glm::degrees(atan2(direction.z, direction.x)) - 90.0F;
  if (targetYaw < 0.0F) {
    targetYaw += 360.0F;
  }
  float localYaw = mLocalPlayer->getYaw();
  float angleDiff = fabs(localYaw - targetYaw);
  angleDiff = fmod(angleDiff + 180.0f, 360.0f) - 180.0f;
  return fabs(angleDiff) <= maxFov / 2.0f;
}
static bool isBot(Player *player) {
  float Pitch = player->getPitch();
  float Yaw = player->getYaw();
  return Yaw == 0.0F && Pitch == 0.0F;
}
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
    gui.SliderFloat("fov", "视角", 0.0F, 360.0F);
    gui.SliderInt("failurerate", "失败率", 0, 100);
    gui.Selectable("priority", "优先级", PriorityItems);
  });
  setOnTick([](Module *module) {
    bool enabled = false;
    float Range = NAN;
    bool swing = false;
    int cps = 0;
    int attackNum = 0;
    bool antibot = false;
    int failurerate = 0;
    float fov = NAN;
    int priority = 0;
    try {
      enabled = module->getGUI().Get<bool>("enabled");
      Range = module->getGUI().Get<float>("range");
      swing = module->getGUI().Get<bool>("swing");
      cps = module->getGUI().Get<int>("cps");
      attackNum = module->getGUI().Get<int>("attackNum");
      antibot = module->getGUI().Get<bool>("antibot");
      fov = module->getGUI().Get<float>("fov");
      failurerate = module->getGUI().Get<int>("failurerate");
      priority = module->getGUI().Get<int>("priority");
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
    mDimension->forEachPlayer([mLocalPlayer, antibot, Range, fov](Player &player) {
      if (&player == mLocalPlayer) {
        return true;
      }
      if (isBot(&player) && antibot) {
        return true;
      }
      float mDistance = mLocalPlayer->getDistance(&player);
      if (mDistance > Range) {
        return true;
      }
      if (!isInFov(mLocalPlayer, &player, fov)) {
        return true;
      }
      PlayerList.push_back(&player);
      return true;
    });
    if (PlayerList.empty()) {
      return;
    }
    GameMode *mGameMode = &mLocalPlayer->getGameMode(); //LocalPlayer
    for (auto *player : PlayerList) {
      if (attackCount >= attackNum) {
        break;
      }
      attackCount++;
      if (failurerate <= 0 || g_dist(g_gen) >= failurerate) {
        mGameMode->attack(*player);
      }
      if (swing) {
        mLocalPlayer->swing();
      }
    }
    LastAttackTime = std::chrono::steady_clock::now();
  });
}