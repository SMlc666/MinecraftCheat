#include "Strafe.hpp"
#include "Helper/Target/Target.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <random>
#include <unordered_map>
#include <vector>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"Mode", 0},        {"Speed", 0.5F},
    {"Range", 5.0F},    {"Priority", 0},     {"AntiBot", false}, {"fov", 360.0F},
    {"distance", 0.1F}, {"changeY", false},  {"slow", false},    {"slowScale", 0.1F},
};
static const std::vector<std::string> StrafeItems = {"LockBack", "Surround"};
static const std::vector<std::string> PriorityItems = {"Distance", "Health", "Random"};
static Player *lastTarget = nullptr;
static float currentSpeed = 0.0f;
cheat::Strafe::Strafe() : Module("Strafe", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) { lastTarget = nullptr; });
  setOnDrawGUI([](Module *module) {
    try {
      module->getGUI().Selectable("Mode", "模式", StrafeItems);
      module->getGUI().SliderFloat("Speed", "速度", 0.01F, 5.0F);
      module->getGUI().SliderFloat("Range", "范围", 1.0F, 15.0F);
      module->getGUI().SliderFloat("fov", "视角", 0.0F, 360.0F);
      module->getGUI().SliderFloat("distance", "玩家距离", 0.0F, 10.0F);
      module->getGUI().CheckBox("changeY", "调整Y轴");
      module->getGUI().CheckBox("slow", "减速");
      if (module->getGUI().Get<bool>("slow")) {
        module->getGUI().SliderFloat("slowScale", "减速比例", 0.01F, 3.0F);
      }
      module->getGUI().CheckBox("AntiBot", "反机器人");
    } catch (...) {
      return;
    }
  });
  setOnTick([](Module *module) {
    try {
      int mode = module->getGUI().Get<int>("Mode");
      int priority = module->getGUI().Get<int>("Priority");
      auto speed = module->getGUI().Get<float>("Speed");
      auto Range = module->getGUI().Get<float>("Range");
      auto fov = module->getGUI().Get<float>("fov");
      bool antibot = module->getGUI().Get<bool>("AntiBot");
      auto distance = module->getGUI().Get<float>("distance");
      bool changeY = module->getGUI().Get<bool>("changeY");
      bool slowEnabled = module->getGUI().Get<bool>("slow");
      float slowScaleVal = module->getGUI().Get<float>("slowScale");

      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        return;
      }
      LocalPlayer *mLocalPlayer = instance->getLocalPlayer();
      if (mLocalPlayer == nullptr) {
        return;
      }
      Dimension *mDimension = mLocalPlayer->mDimension;
      if (mDimension == nullptr) {
        return;
      }
      std::vector<Player *> Players;
      mDimension->forEachPlayer([&](Player &player) {
        return Helper::Target::ProcessPlayer(player, mLocalPlayer, antibot, Range, fov, Players);
      });
      if (Players.empty()) {
        lastTarget = nullptr;
        return;
      }
      if (priority == 0) { // Distance
        std::sort(Players.begin(), Players.end(), [&](Player *a, Player *b) {
          return a->getDistance(mLocalPlayer) < b->getDistance(mLocalPlayer);
        });
      } else if (priority == 1) { // Health
        std::sort(Players.begin(), Players.end(),
                  [&](Player *a, Player *b) { return a->getHealth() > b->getHealth(); });
      } else if (priority == 2) { // Random
        std::shuffle(Players.begin(), Players.end(), std::mt19937(std::random_device()()));
      }
      Player *targetPlayer = Players[0];
      float effectiveSpeed = speed;
      if (slowEnabled) {
        constexpr float ACCELERATION_FACTOR = 0.05f;
        const float baseSpeed = speed * slowScaleVal;
        if (lastTarget != targetPlayer) {
          lastTarget = targetPlayer;
          currentSpeed = baseSpeed;
        } else {
          currentSpeed += (speed - currentSpeed) * ACCELERATION_FACTOR;
          const float minSpeed = baseSpeed * 0.8f;
          if (currentSpeed < minSpeed) {
            currentSpeed = minSpeed;
          }
        }
        effectiveSpeed = currentSpeed;
      }
      glm::vec3 motion = mLocalPlayer->getMotion();
      if (mode == 0) { // LockBack 模式
        glm::vec3 targetPosition = targetPlayer->getPosition();
        float targetYaw = targetPlayer->getYaw();
        float backYaw = glm::mod(targetYaw + 180.0f, 360.0f);
        glm::vec3 offset =
            glm::vec3(-sin(glm::radians(backYaw)), 0.0f, cos(glm::radians(backYaw))) * distance;
        glm::vec3 behindPosition = targetPosition + offset;
        glm::vec3 direction = behindPosition - mLocalPlayer->getPosition();
        float verticalDifference = direction.y;
        direction.y = 0.0f;
        direction = glm::normalize(direction);
        glm::vec3 desiredMotion = direction * effectiveSpeed;

        if (changeY) {
          desiredMotion.y = verticalDifference * effectiveSpeed;
        } else {
          desiredMotion.y = motion.y;
        }
        mLocalPlayer->setMotion(desiredMotion);
      } else if (mode == 1) { // Surround 模式
        glm::vec3 targetPosition = targetPlayer->getPosition();
        float angle = glm::radians(glm::mod(glm::degrees(mLocalPlayer->getYaw()), 360.0f));
        float radius = distance;
        float angleSpeed = 0.05f;
        float circleX = targetPosition.x + radius * cos(angle + angleSpeed);
        float circleZ = targetPosition.z + radius * sin(angle + angleSpeed);

        glm::vec3 direction =
            glm::vec3(circleX, targetPosition.y, circleZ) - mLocalPlayer->getPosition();
        float verticalDifference = direction.y;
        direction.y = 0.0f;
        direction = glm::normalize(direction);
        glm::vec3 desiredMotion = direction * effectiveSpeed;

        if (changeY) {
          desiredMotion.y = verticalDifference * effectiveSpeed;
        } else {
          desiredMotion.y = motion.y;
        }

        mLocalPlayer->setMotion(desiredMotion);
      }
    } catch (...) {
      return;
    }
  });
}
