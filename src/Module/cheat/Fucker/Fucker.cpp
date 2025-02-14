#include "Fucker.hpp"
#include "Helper/Block/Block.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/world/level/GameType.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <string>

#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},       {"shortcut", false},     {"range", 5.0F},
    {"SpoofCreative", false}, {"ThroughWalls", false}, {"Safe", false},
};
glm::ivec3 SafeTargetPos{};
bool SafeHasDestroyedBlock = false;
} // namespace
cheat::Fucker::Fucker() : Module("Fucker", MenuType::WORLD_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("SpoofCreative", "伪装创造模式");
    module->getGUI().SliderFloat("range", "范围", 1.0F, 10.0F);
    module->getGUI().CheckBox("ThroughWalls", "穿墙");
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    try {
      int Range = module->getGUI().Get<float>("range");
      bool SpoofCreative = module->getGUI().Get<bool>("SpoofCreative");
      bool ThroughWalls = module->getGUI().Get<bool>("ThroughWalls");
      bool Safe = module->getGUI().Get<bool>("Safe");
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
      if (SpoofCreative) {
        player->setPlayerGameType(GameType::Creative);
      }
      if (Safe && SafeTargetPos != glm::ivec3(0, 0, 0) &&
          player->getDistance(SafeTargetPos) <= Range &&
          Helper::Block::blockNameHas(SafeTargetPos, "bed")) {
        player->getGameMode().startDestroyBlock(SafeTargetPos, 1, SafeHasDestroyedBlock);
        if (SafeHasDestroyedBlock) {
          player->getGameMode().destroyBlock(SafeTargetPos, 1);
          player->getGameMode().stopDestroyBlock(SafeTargetPos);
          SafeTargetPos = glm::ivec3(0, 0, 0);
          SafeHasDestroyedBlock = false;
        }
      }
      for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
          for (int z = startZ; z <= endZ; ++z) {
            glm::ivec3 targetPos(x, y, z);
            if (!Helper::Block::isAirBlock(targetPos) &&
                Helper::Block::blockNameHas(targetPos, "bed")) {
              if (!ThroughWalls && Helper::Block::hasAdjacentAirBlocks(targetPos)) {
                if (Safe && SafeTargetPos == glm::ivec3(0, 0, 0)) {
                  SafeTargetPos = targetPos;
                } else {
                  player->getGameMode().destroyBlock(targetPos, 1);
                }
              }
            }
          }
        }
      }
      if (SpoofCreative) {
        player->setPlayerGameType(GameType::Default);
      }
    } catch (...) {
      return;
    }
  });
}
