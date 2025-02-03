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
    {"enabled", false}, {"shortcut", false}, {"step", 1}, {"range", 20.0F},
    {"fov", 360.0F},    {"AntiBot", false},  {"Mode", 0}, {"Prioriry", 0}};
static const std::vector<std::string> ModeItems = {"Teleport", "Motion"};
static const std::vector<std::string> PrioriryItems = {
    "Health",
    "Distance",
    "Random",
};
static glm::vec3 originPos{};
cheat::InfiniteAura::InfiniteAura() : Module("InfiniteAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.SliderFloat("range", "范围", 1.0F, 100.0F);
    gui.SliderInt("step", "步骤", 1, 20);
    gui.SliderFloat("fov", "视角", 0.0F, 360.0F);
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

      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr)
        return;

      auto *mLocalPlayer = instance->getLocalPlayer();
      if (mLocalPlayer == nullptr)
        return;

      Dimension *dimension = mLocalPlayer->mDimension;
      if (dimension == nullptr)
        return;

      std::vector<Player *> players;
      dimension->forEachPlayer([&](Player &player) {
        return Helper::Target::ProcessPlayer(player, mLocalPlayer, antibot, range, fov, players);
      });

      if (players.empty())
        return;

      // Prioritization logic
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

      // Check if we are close to the target
      if (glm::distance(selfPos, targetPos) < 1.0F) {
        // Return according to the mode
        if (mode == 0) {
          // Teleport back
          mLocalPlayer->setPosition(originPos);
        } else if (mode == 1) {
          // Motion return, slowly adjust motion back
          mLocalPlayer->setMotion(glm::normalize(originPos - selfPos) * 0.1f);
        }
        originPos = {}; // Clear origin after return
        return;
      }

      if (originPos == glm::vec3{}) {
        originPos = selfPos;
      }

      if (mode == 0) {
        // Teleport mode: interpolate to target position, respecting step
        glm::vec3 moveVec = glm::normalize(targetPos - selfPos) *
                            std::min(static_cast<float>(step), glm::distance(selfPos, targetPos));
        mLocalPlayer->setPosition(selfPos + moveVec);
      } else if (mode == 1) {
        // Motion mode: move smoothly with motion
        glm::vec3 moveVec = glm::normalize(targetPos - selfPos) *
                            std::min(static_cast<float>(step), glm::distance(selfPos, targetPos));
        mLocalPlayer->setMotion(moveVec);
      }
    } catch (...) {
      return;
    }
  });
}