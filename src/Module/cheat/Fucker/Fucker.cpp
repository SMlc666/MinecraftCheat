#include "Fucker.hpp"
#include "Helper/Block/Block.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <string>

#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"range", 5.0F},
};
}
cheat::Fucker::Fucker() : Module("Fucker", MenuType::WORLD_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) { module->getGUI().SliderFloat("range", "范围", 1.0F, 5.0F); });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (!instance)
      return;
    LocalPlayer *player = instance->getLocalPlayer();
    if (!player)
      return;
    float range = module->getGUI().Get<float>("range");
    glm::vec3 pos = player->getPosition();
    for (float x = -range; x <= range; x++) {
      for (float z = -range; z <= range; z++) {
        for (float y = pos.y; y <= range; y++) {
          glm::vec3 blockPos = pos + glm::vec3(x, y, z);
          if (Helper::Block::blockNameHas(pos, "bed")) {
            player->getGameMode().destroyBlock(blockPos, 1);
          }
        }
      }
    }
  });
}
