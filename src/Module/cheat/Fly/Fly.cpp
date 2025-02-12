#include "Fly.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/AbilitiesIndex.hpp"
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
    "Vanilla",
};
static bool inVanillaMode = false;
cheat::Fly::Fly() : Module("Fly", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        return;
      }
      auto *player = instance->getLocalPlayer();
      if (player == nullptr) {
        return;
      }
      int Mode = module->getGUI().Get<int>("Mode");
      if (Mode == 1) {
        inVanillaMode = true;
        player->getAbilities()->setAbility(AbilitiesIndex::MayFly, true);
      }
    } catch (...) {
      return;
    }
  });
  setOnDisable([](Module *module) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        return;
      }
      auto *player = instance->getLocalPlayer();
      if (player == nullptr) {
        return;
      }
      if (inVanillaMode) {
        player->getAbilities()->setAbility(AbilitiesIndex::MayFly, false);
      }
      inVanillaMode = false;
    } catch (...) {
      return;
    }
  });
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