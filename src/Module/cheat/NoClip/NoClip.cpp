#include "NoClip.hpp"
#include "game/minecraft/actor/player/AbilitiesIndex.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
};
}
cheat::NoClip::NoClip() : Module("NoClip", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnTick([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (!instance) {
      return;
    }
    auto *player = instance->getLocalPlayer();
    if (!player) {
      return;
    }
    player->getAbilities()->setAbility(AbilitiesIndex::NoClip, true);
  });
  setOnDisable([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (!instance) {
      return;
    }
    auto *player = instance->getLocalPlayer();
    if (!player) {
      return;
    }
    player->getAbilities()->setAbility(AbilitiesIndex::NoClip, false);
  });
}