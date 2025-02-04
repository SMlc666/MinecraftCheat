#include "InfiniteAura.hpp"
#include "Helper/Target/Target.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "glm/geometric.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <cstddef>
#include <random>
#include <unordered_map>
#include <algorithm>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"step", 1}, {"range", 20.0F}, {"fov", 360.0F},
    {"AntiBot", false}, {"Distance", 1.0F},  {"Mode", 0}, {"Prioriry", 0}};
static const std::vector<std::string> ModeItems = {"Teleport", "Motion"};
static const std::vector<std::string> PrioriryItems = {
    "Health",
    "Distance",
    "Random",
};
static glm::vec3 originPos{};
static glm::vec3 moveDirection{};
static int remainingSteps = 0;
cheat::InfiniteAura::InfiniteAura() : Module("InfiniteAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.SliderFloat("range", "范围", 1.0F, 100.0F);
    gui.SliderInt("step", "步骤", 1, 20);
    gui.SliderFloat("fov", "视角", 0.0F, 360.0F);
    gui.SliderFloat("Distance", "返回距离", 0.1F, 10.0F);
    gui.CheckBox("AntiBot", "反机器人");
    gui.Selectable("Mode", "模式", ModeItems);
    gui.Selectable("Prioriry", "优先级", PrioriryItems);
  });
  setOnTick([](Module *module) {
    try {
      auto &gui = module->getGUI();
      bool antibot = gui.Get<bool>("AntiBot");
      auto fov = gui.Get<float>("fov");
      auto range = gui.Get<float>("range");
      auto step = gui.Get<int>("step");
      auto mode = gui.Get<int>("Mode");
      auto priority = gui.Get<int>("Prioriry");
      auto distance = gui.Get<float>("Distance");
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        return;
      }
      auto *mLocalPlayer = instance->getLocalPlayer();
      if (mLocalPlayer == nullptr) {
        return;
      }
      Dimension *dimension = mLocalPlayer->mDimension;
      if (dimension == nullptr) {
        return;
      }
      std::vector<Player *> players;
      dimension->forEachPlayer([&](Player &player) {
        return Helper::Target::ProcessPlayer(player, mLocalPlayer, antibot, range, fov, players);
      });
      if (players.empty()) {
        return;
      }
      if (priority == 0) {
        std::sort(players.begin(), players.end(),
                  [](Player *a, Player *b) { return a->getHealth() < b->getHealth(); });
      } else if (priority == 1) {
        std::sort(players.begin(), players.end(), [mLocalPlayer](Player *a, Player *b) {
          return mLocalPlayer->getDistance(a) < mLocalPlayer->getDistance(b);
        });
      } else if (priority == 2) {
        std::shuffle(players.begin(), players.end(), std::mt19937(std::random_device()()));
      }
      Player *target = players.front();
      glm::vec3 targetPos = target->getPosition();
      glm::vec3 selfPos = mLocalPlayer->getPosition();
      if (glm::distance(selfPos, targetPos) < distance) {
        if (mode == 0) {
          mLocalPlayer->setPosition(originPos);
        } else if (mode == 1) {
          mLocalPlayer->setMotion(glm::normalize(originPos - selfPos) * 0.1f);
        }
        originPos = {};
        return;
      }
      if (originPos == glm::vec3{}) {
        originPos = selfPos;
        glm::vec3 toTarget = targetPos - selfPos;
        float totalDistance = glm::length(toTarget);
        remainingSteps = static_cast<int>(std::ceil(totalDistance / step));
        if (remainingSteps > 0) {
          moveDirection = glm::normalize(toTarget);
        } else {
          moveDirection = glm::vec3{};
        }
      }
      if (remainingSteps > 0) {
        float currentStep = std::min(static_cast<float>(step), glm::distance(selfPos, targetPos));
        if (mode == 0) {
          mLocalPlayer->setPosition(selfPos + moveDirection * currentStep);
        } else if (mode == 1) {
          mLocalPlayer->setMotion(moveDirection * currentStep);
        }
        --remainingSteps;
        if (remainingSteps <= 0) {
          moveDirection = glm::vec3{};
          originPos = glm::vec3{};
        }
      }
    } catch (...) {
      return;
    }
  });
}