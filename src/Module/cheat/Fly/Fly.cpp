#include "Fly.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
#include <vector>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"Mode", 0},
};
static const std::vector<std::string> ModeItems = {
    "SameY",
};
cheat::Fly::Fly() : Module("Fly", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().Selectable("Mode", "模式", ModeItems); });
  setOnTick([](Module *module) {
    int Mode{};
    try {
      Mode = module->getGUI().Get<int>("Mode");
    } catch (...) {
      return;
    }
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }
    auto *player = instance->getLocalPlayer();
    if (player == nullptr) {
      return;
    }
    if (Mode == 0) {
      glm::vec3 motion = player->getMotion();
      motion.y = 0;
      player->setMotion(motion);
    }
  });
}