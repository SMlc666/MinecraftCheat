#include "Speed.hpp"
#include "game/common/SubClientId.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include "Module.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"Mode", 0}, {"Speed", 1.0F}, {"HopMotionY", 0.42F}};
static const std::vector<std::string> ModeItems = {
    "Vanilla",
    "Custom",
};
static const float NormalSpeed = 1.0F;
cheat::Speed::Speed() : Module("Speed", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnTick([](Module *module) {
    float speed{};
    try {
      speed = module->getGUI().Get<float>("Speed");
    } catch (...) {
      return;
    }
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }
    LocalPlayer *player = instance->getLocalPlayer();
    if (player == nullptr) {
      return;
    }
    player->setSpeed(speed);
  });
  setOnDisable([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }
    LocalPlayer *player = instance->getLocalPlayer();
    if (player == nullptr) {
      return;
    }
    player->setSpeed(NormalSpeed);
  });
  setOnDrawGUI([](Module *module) {
    try {
      module->getGUI().Selectable("Mode", "模式", ModeItems);
      module->getGUI().SliderFloat("Speed", "速度", 0.1F, 10.0F);
      if (ImGui::TreeNode("Custom")) {
        module->getGUI().SliderFloat("HopMotionY", "跳跃高度", 0.1F, 1.0F);
        ImGui::TreePop();
      }
    } catch (...) {
      return;
    }
  });
  setOnMove([](Module *module, MoveInputHandler *handler) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        return;
      }
      LocalPlayer *player = instance->getLocalPlayer();
      if (player == nullptr) {
        return;
      }
      int mode = module->getGUI().Get<int>("Mode");
      float HopMotionY = module->getGUI().Get<float>("HopMotionY");
      float speed = module->getGUI().Get<float>("Speed");
      glm::vec3 motion = player->getMotion();
      float calcYaw = player->getYaw();
      bool isOnGround = ActorCollision::isOnGround(player->mEntityContext);
      if (mode == 1) {
        if (handler->isMove()) {
          if (isOnGround) {
            motion.y = HopMotionY;
          }
        }
        if (handler->leftInput())
          calcYaw -= 90.f;
        if (handler->rightInput())
          calcYaw += 90.f;
        if (handler->forwardLeftInput())
          calcYaw -= 45.f;
        if (handler->forwardRightInput())
          calcYaw += 45.f;
        calcYaw = glm::radians(calcYaw);
        motion.x = -sin(calcYaw) * speed;
        motion.z = cos(calcYaw) * speed;
        if (handler->backwardInput()) {
          motion.x = -motion.x;
          motion.z = -motion.z;
        }
      } else {
        motion.x = 0.0F;
        motion.z = 0.0F;
      }
      player->setMotion(motion);
      player->lerpMotion(motion);
    } catch (...) {
      return;
    }
  });
}