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
    {"enabled", false}, {"shortcut", false}, {"Mode", 0},     {"Speed", 0.5F},   {"Range", 5.0F},
    {"Priority", 0},    {"AntiBot", false},  {"fov", 360.0F}, {"distance", 0.1F}};
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
      // 在onTick函数中的模式判断处添加以下代码
      if (mode == 0) { // LockBack 模式
        Player *target = Players[0];

        // 获取目标和本地玩家的位置
        glm::vec3 targetPos = target->getPosition();
        glm::vec3 playerPos = mLocalPlayer->getPosition();

        // 获取目标的朝向
        float targetYaw = target->getYaw();

        // 计算目标背后的位置 (以目标为中心，距离为distance，角度为目标的朝向+180度)
        float angleInRadians = (targetYaw + 180.0f) * glm::pi<float>() / 180.0f;
        glm::vec3 desiredPos = targetPos + glm::vec3(sin(angleInRadians) * distance, 0,
                                                     cos(angleInRadians) * distance);

        // 计算需要移动的向量
        glm::vec3 moveVector = desiredPos - playerPos;

        // 标准化移动向量并应用速度
        float length = sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y +
                            moveVector.z * moveVector.z);
        if (length > 0.0001f) {
          moveVector = moveVector * (speed / length);
        }

        // 添加Y轴移动以使玩家与目标在同一水平面
        moveVector.y = (targetPos.y - playerPos.y) * speed;

        // 设置移动
        mLocalPlayer->setMotion(moveVector);
      } else if (mode == 1) { // Surround 模式
        Player *target = Players[0];

        // 获取目标和本地玩家的位置
        glm::vec3 targetPos = target->getPosition();
        glm::vec3 playerPos = mLocalPlayer->getPosition();

        // 计算当前tick的角度 (随时间变化)
        static float angle = 0.0f;
        angle += speed * 0.2f; // 控制旋转速度
        if (angle > 360.0f)
          angle -= 360.0f;

        float angleInRadians = angle * glm::pi<float>() / 180.0f;

        // 计算期望位置 (以目标为中心，距离为distance)
        glm::vec3 desiredPos = targetPos + glm::vec3(sin(angleInRadians) * distance, 0,
                                                     cos(angleInRadians) * distance);

        // 计算需要移动的向量
        glm::vec3 moveVector = desiredPos - playerPos;

        // 标准化移动向量并应用速度
        float length = sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y +
                            moveVector.z * moveVector.z);
        if (length > 0.0001f) {
          moveVector = moveVector * (speed / length);
        }

        // 添加Y轴移动以使玩家与目标在同一水平面
        moveVector.y = (targetPos.y - playerPos.y) * speed;

        // 设置移动
        mLocalPlayer->setMotion(moveVector);
      }
    } catch (...) {
      return;
    }
  });
}