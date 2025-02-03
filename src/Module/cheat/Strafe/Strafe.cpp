#include "Strafe.hpp"
#include "Helper/Target/Target.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <random>
#include <unordered_map>
#include <vector>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"Mode", 0},        {"Speed", 0.5F},
    {"Range", 5.0F},    {"Priority", 0},     {"AntiBot", false}, {"fov", 360.0F},
    {"distance", 0.1F}, {"changeY", false},
};
static const std::vector<std::string> StrafeItems = {"LockBack", "Surround"};
static const std::vector<std::string> PriorityItems = {"Distance", "Health", "Random"};
cheat::Strafe::Strafe() : Module("Strafe", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().Selectable("Mode", "模式", StrafeItems);
    module->getGUI().Selectable("Priority", "优先级", PriorityItems);
    module->getGUI().SliderFloat("Speed", "速度", 0.1F, 5.0F);
    module->getGUI().SliderFloat("Range", "范围", 1.0F, 15.0F);
    module->getGUI().SliderFloat("fov", "视角", 0.0F, 360.0F);
    module->getGUI().SliderFloat("distance", "玩家距离", 0.0F, 10.0F);
    module->getGUI().CheckBox("changeY", "调整Y轴");
    module->getGUI().CheckBox("AntiBot", "反机器人");
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
      glm::vec3 motion = mLocalPlayer->getMotion();
      if (mode == 0) { // LockBack 模式
        // 获取玩家的目标位置
        Player *targetPlayer = Players[0];
        glm::vec3 targetPosition = targetPlayer->getPosition();
        float targetYaw = targetPlayer->getYaw(); // 获取目标的方向

        // 计算目标背后的位置 (通过目标的 yaw 角度反向计算)
        float backYaw = glm::mod(targetYaw + 180.0f, 360.0f); // 将 yaw 加上 180 度，表示目标背后
        glm::vec3 offset =
            glm::vec3(-sin(glm::radians(backYaw)), 0.0f, cos(glm::radians(backYaw))) * distance;
        glm::vec3 behindPosition = targetPosition + offset;

        // 计算当前玩家与目标背后位置的向量
        glm::vec3 direction = behindPosition - mLocalPlayer->getPosition();

        // 调整 y 轴的速度，使玩家保持在一个水平面上
        float verticalDifference = direction.y;
        direction.y = 0.0f; // 只关注水平面的方向
        glm::normalize(direction);

        // 调整玩家的速度以使其向目标背后移动
        glm::vec3 desiredMotion = direction * speed;

        if (changeY) {
          desiredMotion.y = verticalDifference * speed;
        } else {
          desiredMotion.y = motion.y;
        }
        // 更新玩家的运动
        mLocalPlayer->setMotion(desiredMotion);
      } else if (mode == 1) { // Surround 模式
        // 获取玩家的目标位置
        Player *targetPlayer = Players[0];
        glm::vec3 targetPosition = targetPlayer->getPosition();

        // 计算围绕目标转圈的运动
        float angle = glm::radians(
            glm::mod(glm::degrees(mLocalPlayer->getYaw()), 360.0f)); // 让角度保持在 [0, 360]
        float radius = distance;                                     // 设定转圈的半径
        float angleSpeed = 0.05f; // 每个tick的转动角度速度，您可以通过速度参数来调节

        // 计算围绕目标转圈的新的 x 和 z 坐标
        float circleX = targetPosition.x + radius * cos(angle + angleSpeed);
        float circleZ = targetPosition.z + radius * sin(angle + angleSpeed);

        // 计算从玩家当前位置到目标位置的向量
        glm::vec3 direction =
            glm::vec3(circleX, targetPosition.y, circleZ) - mLocalPlayer->getPosition();

        // 调整y轴的速度，使玩家保持水平
        float verticalDifference = direction.y;
        direction.y = 0.0f;
        glm::normalize(direction);

        // 调整玩家的速度
        glm::vec3 desiredMotion = direction * speed;

        if (changeY) {
          desiredMotion.y = verticalDifference * speed;
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