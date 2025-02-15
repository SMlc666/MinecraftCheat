#include "Fucker.hpp"
#include "Helper/Block/Block.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/actor.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/world/level/GameType.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <string>

#include <unordered_map>
#include <queue>
#include <chrono>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},      {"shortcut", false}, {"range", 5.0F},  {"SpoofCreative", false},
    {"ThroughWalls", false}, {"Safe", false},     {"cooldown", 500}};
glm::ivec3 SafeTargetPos = glm::ivec3(0, 0, 0);
bool SafeHasDestroyedBlock = false;
std::queue<glm::ivec3> targetQueue;
std::chrono::time_point<std::chrono::steady_clock> lastDestroyTime;
} // namespace
cheat::Fucker::Fucker() : Module("Fucker", MenuType::WORLD_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("SpoofCreative", "伪装创造模式");
    module->getGUI().SliderFloat("range", "范围", 1.0F, 10.0F);
    module->getGUI().CheckBox("ThroughWalls", "穿墙");
    module->getGUI().SliderInt("cooldown", "冷却时间(ms)", 0, 5000);
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    try {
      auto now = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDestroyTime).count() <
          module->getGUI().Get<int>("cooldown")) {
        return;
      }
      float Range = module->getGUI().Get<float>("range");
      bool SpoofCreative = module->getGUI().Get<bool>("SpoofCreative");
      bool ThroughWalls = module->getGUI().Get<bool>("ThroughWalls");
      bool Safe = module->getGUI().Get<bool>("Safe");
      ClientInstance *client = runtimes::getClientInstance();
      if (!client)
        return;
      LocalPlayer *player = client->getLocalPlayer();
      if (!player)
        return;
      GameMode *gameMode = &player->getGameMode();
      if (!gameMode)
        return;
      glm::vec3 playerPos = player->getPosition();
      if (SpoofCreative)
        player->setPlayerGameType(GameType::Creative);
      if (Safe && !SafeHasDestroyedBlock && SafeTargetPos != glm::ivec3(0, 0, 0) &&
          player->getDistance(SafeTargetPos) <= Range) {
        if (!gameMode->startDestroyBlock(SafeTargetPos, 1, SafeHasDestroyedBlock)) {
          SafeTargetPos = glm::ivec3(0, 0, 0);
          SafeHasDestroyedBlock = false;
          gameMode->stopDestroyBlock(SafeTargetPos);
        }
      } else {
        if (SafeTargetPos != glm::ivec3(0, 0, 0)) {
          gameMode->stopDestroyBlock(SafeTargetPos);
        }
        SafeTargetPos = glm::ivec3(0, 0, 0);
        SafeHasDestroyedBlock = false;
      }
      int fromX = playerPos.x - Range;
      int toX = playerPos.x + Range;
      int fromY = playerPos.y - Range;
      int toY = playerPos.y + Range;
      int fromZ = playerPos.z - Range;
      int toZ = playerPos.z + Range;
      for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
          for (int z = fromZ; z <= toZ; z++) {
            glm::ivec3 pos(x, y, z);
            if (Helper::Block::blockNameHas(pos, "bed")) {
              if (!ThroughWalls && !Helper::Block::hasAdjacentAirBlocks(pos)) {
                continue;
              }
              if (Safe) {
                SafeTargetPos = pos;
              } else {
                gameMode->destroyBlock(pos, 1);
                lastDestroyTime = std::chrono::steady_clock::now();
              }
            }
          }
        }
      }
      player->setPlayerGameType(GameType::Default);
    } catch (...) {
      return;
    }
  });
}
