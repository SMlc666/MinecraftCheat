#include "AntiVoid.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "glm/ext/vector_float3.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"distance", 2.0f},
    {"Xoffest", 0.0f},  {"Yoffest", 0.0f},   {"Zoffest", 0.0f},
};
glm::vec3 savePos{};
} // namespace
cheat::AntiVoid::AntiVoid() : Module("AntiVoid", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("distance", "距离", 1.0F, 10.0F);
    if (ImGui::TreeNode("偏移")) {
      module->getGUI().SliderFloat("Xoffest", "X轴偏移", -10.0f, 10.0f);
      module->getGUI().SliderFloat("Yoffest", "Y轴偏移", -10.0f, 10.0f);
      module->getGUI().SliderFloat("Zoffest", "Z轴偏移", -10.0f, 10.0f);
      ImGui::TreePop();
    }
  });
  setOnTick([](Module *module) {
    try {
      float distance = module->getGUI().Get<float>("distance");
      float Xoffest = module->getGUI().Get<float>("Xoffest");
      float Yoffest = module->getGUI().Get<float>("Yoffest");
      float Zoffest = module->getGUI().Get<float>("Zoffest");
      glm::vec3 offest(Xoffest, Yoffest, Zoffest);
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance) {
        return;
      }
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player) {
        return;
      }
      glm::vec3 pos = player->getPosition();
      bool isOnGround = ActorCollision::isOnGround(player->mEntityContext);
      if (isOnGround) {
        savePos = pos;
      } else {
        float fallDistance = player->getFallDistance();
        if (fallDistance > distance) {
          player->setPosition(savePos + offest);
        }
      }
    } catch (...) {
      return;
    }
  });
}