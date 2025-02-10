#include "ChestAura.hpp"
#include "Helper/Block/Block.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "glm/ext/vector_int3.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"range", 5.0f},          {"shortcut", false},
    {"dupOpen", false}, {"uniqueDistance", 6.0F},
};
std::vector<glm::ivec3> chestPositions;
} // namespace
cheat::ChestAura::ChestAura() : Module("ChestAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    try {
      module->getGUI().SliderFloat("range", "范围", 1.0F, 10.0F);
      module->getGUI().CheckBox("dupOpen", "重复开箱");
      module->getGUI().SliderFloat("uniqueDistance", "不重复距离", 1.0F, 10.0F);
    } catch (...) {
      return;
    }
  });
  setOnRender([](Module *module) {
    try {
      float range = module->getGUI().Get<float>("range");
      bool dupOpen = module->getGUI().Get<bool>("dupOpen");
      float uniqueDistance = module->getGUI().Get<float>("uniqueDistance");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;
      if (dupOpen) {
        for (auto it = chestPositions.begin(); it != chestPositions.end();) {
          if (player->getDistance(glm::vec3(*it)) >= uniqueDistance) {
            it = chestPositions.erase(it);
          } else {
            ++it;
          }
        }
      }
      glm::vec3 playerPos = player->getPosition();
      int fromX = playerPos.x - range;
      int fromY = playerPos.y - range;
      int fromZ = playerPos.z - range;
      int toX = playerPos.x + range;
      int toY = playerPos.y + range;
      int toZ = playerPos.z + range;
      for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
          for (int z = fromZ; z <= toZ; z++) {
            glm::ivec3 targetPos(x, y, z);
            if (Helper::Block::blockNameHas(targetPos, "chest") &&
                player->canOpenContainerScreen() &&
                std::find(chestPositions.begin(), chestPositions.end(), targetPos) ==
                    chestPositions.end()) {
              player->getGameMode().buildBlock(targetPos, 1);
            }
          }
        }
      }
    } catch (...) {
      return;
    }
  });
}