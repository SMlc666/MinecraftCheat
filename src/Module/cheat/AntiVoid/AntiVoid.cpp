#include "AntiVoid.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "glm/ext/vector_float3.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"distance", 2.0f},
};
glm::vec3 savePos{};
} // namespace
cheat::AntiVoid::AntiVoid() : Module("AntiVoid", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI(
      [](Module *module) { module->getGUI().SliderFloat("distance", "距离", 1.0F, 10.0F); });
  setOnTick([](Module *module) {
    try {
      float distance = module->getGUI().Get<float>("distance");
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
          player->setPosition(savePos);
        }
      }
    } catch (...) {
      return;
    }
  });
}