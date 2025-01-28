#include "Velocity.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "glm/ext/vector_float3.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <any>
#include <unordered_map>
class Actor;
static Module *g_md;
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"horizontalModify", 0.0F},
    {"verticalModify", 0.0F},
};
MemTool::Hook Actor_lerpMotion_;
static void Actor_lerpMotion(Actor *actor, glm::vec3 &Motion) {
  try {
    bool enabled = g_md->getGUI().Get<bool>("enabled");
    if (enabled) {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance != nullptr) {
        LocalPlayer *player = instance->getLocalPlayer();
        if (player == actor) {
          glm::vec3 originalMotion = actor->getMotion();
          auto horizontalModify = g_md->getGUI().Get<float>("horizontalModify");
          auto verticalModify = g_md->getGUI().Get<float>("verticalModify");
          Motion = {originalMotion.x + Motion.x * horizontalModify,
                    originalMotion.y + Motion.y * verticalModify,
                    originalMotion.z + Motion.z * horizontalModify};
        }
      }
    }
  } catch (const std::exception &e) {
  }
  Actor_lerpMotion_.call<void>(actor, Motion);
}
cheat::Velocity::Velocity() : Module("Velocity", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    g_md = module;
    void *lerpMotion = getSign<void *>("Actor::lerpMotion");
    if (lerpMotion != nullptr) {
      Actor_lerpMotion_ =
          MemTool::Hook(lerpMotion, reinterpret_cast<void *>(Actor_lerpMotion), nullptr, false);
    }
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("horizontalModify", "水平", 0.0F, 1.0F);
    module->getGUI().SliderFloat("verticalModify", "垂直", 0.0F, 1.0F);
  });
}