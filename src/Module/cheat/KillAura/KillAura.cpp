#include "KillAura.hpp"
#include <cmath>
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <chrono>
#include <string>
#include <unordered_map>
#include <random>
#include "Helper/Rotation/rotation.hpp"
static const std::vector<std::string> PriorityItems = {"Health", "Distance", "Random"};
static const std::vector<std::string> RotationItems = {"Lock", "Approximate"};
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"mincps", 10},   {"maxcps", 20},
    {"range", 5.0F},    {"swing", false},    {"attackNum", 1}, {"antibot", false},
    {"fov", 180.0F},    {"failurerate", 0},  {"priority", 0},  {"rotation", false},
    {"rotationMode", 0}};
static std::vector<Player *> PlayerList = {};
static std::chrono::steady_clock::time_point LastAttackTime = std::chrono::steady_clock::now();
static std::random_device g_rd;
static std::uniform_int_distribution<> g_dist(0, 100);
static std::mt19937 g_gen(g_rd());
static Player *g_Target{};
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
  angleDiff = fmod(angleDiff + 180.0F, 360.0F) - 180.0F;
  return fabs(angleDiff) <= maxFov / 2.0F;
}
static bool isBot(Player *player) {
  float Pitch = player->getPitch();
  float Yaw = player->getYaw();
  return Yaw == 0.0F && Pitch == 0.0F;
}
static bool ProcessPlayer(Player &player, LocalPlayer *localPlayer, bool antibot, float range,
                          float fov, std::vector<Player *> &playerList) {
  if (&player == localPlayer) {
    return true;
  }

  if (isBot(&player) && antibot) {
    return true;
  }

  const float distance = localPlayer->getDistance(&player);
  if (distance > range) {
    return true;
  }

  if (!isInFov(localPlayer, &player, fov)) {
    return true;
  }

  if (!player.isAlive() || player.getHealth() <= 0) {
    return true;
  }

  playerList.push_back(&player);
  return true;
}
bool hasPlayer(Dimension *dimension, Player *targetPlayer, LocalPlayer *localPlayer, bool antibot,
               float range, float fov) {
  bool foundPlayer = false;
  std::vector<Player *> playerList;
  dimension->forEachPlayer([&](Player &player) {
    if (&player == targetPlayer &&
        ProcessPlayer(player, localPlayer, antibot, range, fov, playerList)) {
      foundPlayer = true;
    }
    return true;
  });

  return foundPlayer;
}

cheat::KillAura::KillAura() : Module("KillAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) { g_Target = nullptr; });
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.SliderFloat("range", "范围", 0.0F, 10.0F);
    gui.SliderInt("mincps", "最小攻击速度", 1, 20, [module](int value) {
      int maxcps = module->getGUI().Get<int>("maxcps");
      if (value > maxcps) {
        module->getGUI().Set<int>("maxcps", value);
      }
    });
    gui.SliderInt("maxcps", "最大攻击速度", 1, 20);
    gui.SliderInt("attackNum", "攻击数量", 1, 20);
    gui.CheckBox("swing", "挥手");
    gui.CheckBox("antibot", "反机器人");
    gui.SliderFloat("fov", "视角", 0.0F, 360.0F);
    gui.SliderInt("failurerate", "失败率", 0, 100);
    gui.Selectable("priority", "优先级", PriorityItems);
    if (ImGui::TreeNode("Rotation")) {
      gui.CheckBox("rotation", "转头");
      gui.Selectable("rotationMode", "转头模式", RotationItems);
      ImGui::TreePop();
    }
  });
  setOnTick([](Module *module) {
    bool enabled = false;
    float Range = NAN;
    bool swing = false;
    int mincps = 0;
    int maxcps = 0;
    int attackNum = 0;
    bool antibot = false;
    int failurerate = 0;
    float fov = NAN;
    int priority = 0;
    try {
      enabled = module->getGUI().Get<bool>("enabled");
      Range = module->getGUI().Get<float>("range");
      swing = module->getGUI().Get<bool>("swing");
      mincps = module->getGUI().Get<int>("mincps");
      maxcps = module->getGUI().Get<int>("maxcps");
      attackNum = module->getGUI().Get<int>("attackNum");
      antibot = module->getGUI().Get<bool>("antibot");
      failurerate = module->getGUI().Get<int>("failurerate");
      fov = module->getGUI().Get<float>("fov");
      priority = module->getGUI().Get<int>("priority");
    } catch (const std::exception &e) {
      return;
    }
    int attackCount = 0;
    auto now = std::chrono::steady_clock::now();
    int rmcps = std::uniform_int_distribution<>(std::min(mincps, maxcps), maxcps)(g_gen);
    auto interval = std::chrono::milliseconds(1000 / rmcps);
    if (!enabled) {
      return;
    }
    if (now - LastAttackTime < interval) {
      return;
    }
    ClientInstance *mInstance = runtimes::getClientInstance();
    if (mInstance == nullptr) {
      return;
    }
    LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
    if (mLocalPlayer == nullptr) {
      return;
    }
    Dimension *mDimension = mLocalPlayer->mDimension;
    if (mDimension == nullptr) {
      return;
    }
    PlayerList.clear();
    mDimension->forEachPlayer([=](Player &player) {
      return ProcessPlayer(player, mLocalPlayer, antibot, Range, fov, PlayerList);
    });
    if (!PlayerList.empty()) {
      g_Target = nullptr;
      if (priority == 0) {
        std::sort(PlayerList.begin(), PlayerList.end(),
                  [](Player *a, Player *b) { return a->getHealth() < b->getHealth(); });
      } else if (priority == 1) {
        std::sort(PlayerList.begin(), PlayerList.end(), [mLocalPlayer](Player *a, Player *b) {
          return mLocalPlayer->getDistance(a) < mLocalPlayer->getDistance(b);
        });
      } else if (priority == 2) {
        std::shuffle(PlayerList.begin(), PlayerList.end(), g_gen);
      }
    } else {
      return;
    }
    GameMode *mGameMode = &mLocalPlayer->getGameMode(); //LocalPlayer
    bool hasAttacked = false;
    for (auto *player : PlayerList) {
      if (attackCount >= attackNum) {
        break;
      }
      attackCount++;
      if (failurerate <= 0 || g_dist(g_gen) >= failurerate) {
        mGameMode->attack(*player);
        g_Target = player;
        hasAttacked = true;
      }
      if (swing) {
        mLocalPlayer->swing();
      }
    }
    if (hasAttacked) {
      LastAttackTime = std::chrono::steady_clock::now();
    }
  });
  setOnRender([](Module *module) {
    using namespace Helper;
    bool enabled = false;
    bool rotation = false;
    bool antibot = false;
    float fov = NAN;
    float Range = NAN;
    int rotationMode = 0;
    try {
      enabled = module->getGUI().Get<bool>("enabled");
      rotation = module->getGUI().Get<bool>("rotation");
      rotationMode = module->getGUI().Get<int>("rotationMode");
      antibot = module->getGUI().Get<bool>("antibot");
      fov = module->getGUI().Get<float>("fov");
      Range = module->getGUI().Get<float>("range");
    } catch (const std::exception &e) {
      return;
    }
    if (g_Target == nullptr) {
      return;
    }
    if (!enabled || !rotation) {
      return;
    }
    ClientInstance *mInstance = runtimes::getClientInstance();
    if (mInstance == nullptr) {
      return;
    }
    LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
    if (mLocalPlayer == nullptr) {
      return;
    }
    if (!hasPlayer(mLocalPlayer->mDimension, g_Target, mLocalPlayer, antibot, Range, fov)) {
      return;
    }
    glm::vec3 localPos = mLocalPlayer->getPosition();
    glm::vec3 targetPos = g_Target->getPosition();
    Rotation::Rotation aimTarget = Rotation::toRotation(localPos, targetPos);
    Rotation::Rotation last = {mLocalPlayer->getPitch(), mLocalPlayer->getYaw()};
    switch (rotationMode) {
    case 0:
      mLocalPlayer->setPitch(aimTarget.pitch);
      mLocalPlayer->setYaw(aimTarget.yaw);
      break;
    case 1:
      aimTarget.pitch /= 2.0F;
      float diff = Rotation::getRotationDifference(last, aimTarget);
      if (diff >= 50.0F) {
        mLocalPlayer->setPitch((aimTarget.pitch - last.pitch) / 0.8F + last.pitch);
        mLocalPlayer->setYaw((aimTarget.yaw - last.yaw) / 0.8F + last.yaw);
      }
      break;
    }
  });
}