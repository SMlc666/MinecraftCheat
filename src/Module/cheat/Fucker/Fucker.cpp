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
    try {
      int Range = module->getGUI().Get<int>("range");
      ClientInstance *client = runtimes::getClientInstance();
      if (!client)
        return;
      LocalPlayer *player = client->getLocalPlayer();
      if (!player)
        return;
      glm::ivec3 pos = player->getPosition();
      int startX = pos.x - Range;
      int startY = pos.y - Range;
      int startZ = pos.z - Range;
      int endX = pos.x + Range;
      int endY = pos.y + Range;
      int endZ = pos.z + Range;
      for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
          for (int z = startZ; z <= endZ; ++z) {
            glm::ivec3 targetPos(x, y, z);
            if (!Helper::Block::isAirBlock(targetPos) &&
                Helper::Block::blockNameHas(targetPos, "bed")) {
              player->getGameMode().destroyBlock(targetPos, 1);
            }
          }
        }
      }
    } catch (...) {
      return;
    }
  });
}
