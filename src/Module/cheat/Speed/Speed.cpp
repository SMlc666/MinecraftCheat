#include "Speed.hpp"
#include "game/common/SubClientId.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "Module.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"Mode", 0}, {"Speed", 1.0F}};
static const std::vector<std::string> ModeItems = {
    "Vanilla",
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
    module->getGUI().Selectable("Mode", "模式", ModeItems);
    module->getGUI().SliderFloat("Speed", "速度", 0.1F, 10.0F);
  });
}