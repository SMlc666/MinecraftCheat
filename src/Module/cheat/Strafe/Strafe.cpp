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
    {"Distance", 5.0F}, {"Priority", 0},     {"AntiBot", false}, {"fov", 360.0F},
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
    module->getGUI().SliderFloat("Distance", "距离", 1.0F, 15.0F);
    module->getGUI().SliderFloat("fov", "视角", 0.0F, 360.0F);
    module->getGUI().CheckBox("AntiBot", "反机器人");
  });
  setOnTick([](Module *module) {
    try {
      int mode = module->getGUI().Get<int>("Mode");
      int priority = module->getGUI().Get<int>("Priority");
      auto speed = module->getGUI().Get<float>("Speed");
      auto distance = module->getGUI().Get<float>("Distance");
      auto fov = module->getGUI().Get<float>("fov");
      bool antibot = module->getGUI().Get<bool>("AntiBot");
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
        return Helper::Target::ProcessPlayer(player, mLocalPlayer, antibot, distance, fov, Players);
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
      // 在onTick函数中的模式判断处添加以下代码
      if (mode == 0) { // LockBack 模式
        Player *target = Players[0];
        glm::vec3 targetPos = target->getPosition();
        float targetYaw = target->getYaw();

        // 计算目标面朝方向（将yaw转换为三维向量）
        float yawRad = glm::radians(targetYaw);
        glm::vec3 direction(-sin(yawRad), 0, cos(yawRad)); // 方向向量

        // 计算目标背后的理想位置
        glm::vec3 desiredPos = targetPos - direction * distance;
        desiredPos.y = mLocalPlayer->getPosition().y; // 保持当前高度

        // 计算运动向量
        glm::vec3 delta = desiredPos - mLocalPlayer->getPosition();
        delta.y = 0; // 仅水平移动

        if (glm::length(delta) > 0.1f) {
          glm::vec3 motionDir = glm::normalize(delta);
          glm::vec3 newMotion = motionDir * speed;
          mLocalPlayer->setMotion(glm::vec3(newMotion.x, mLocalPlayer->getMotion().y, newMotion.z));
        } else {
          mLocalPlayer->setMotion(glm::vec3(0, mLocalPlayer->getMotion().y, 0));
        }
      } else if (mode == 1) { // Surround 模式
        Player *target = Players[0];
        glm::vec3 targetPos = target->getPosition();

        // 使用静态变量保持旋转角度（针对每个目标需要优化）
        static float surroundAngle = 0.0f;

        // 基于线速度计算角度增量（物理正确的圆周运动）
        if (distance > 0.1f) {
          float angularSpeed = (speed / distance) * (180.0f / glm::pi<float>());
          surroundAngle += angularSpeed;
        }
        surroundAngle = fmod(surroundAngle, 360.0f);

        // 计算圆周坐标
        float radian = glm::radians(surroundAngle);
        float xOffset = cos(radian) * distance;
        float zOffset = sin(radian) * distance;

        // 计算目标位置（保持当前Y轴高度）
        glm::vec3 desiredPos = targetPos + glm::vec3(xOffset, 0, zOffset);
        desiredPos.y = mLocalPlayer->getPosition().y;

        // 计算运动向量
        glm::vec3 delta = desiredPos - mLocalPlayer->getPosition();
        delta.y = 0; // 仅水平移动

        if (glm::length(delta) > 0.1f) {
          glm::vec3 motionDir = glm::normalize(delta);
          glm::vec3 newMotion = motionDir * speed;
          mLocalPlayer->setMotion(glm::vec3(newMotion.x, mLocalPlayer->getMotion().y, newMotion.z));
        } else {
          mLocalPlayer->setMotion(glm::vec3(0, mLocalPlayer->getMotion().y, 0));
        }
      }
    } catch (...) {
      return;
    }
  });
}