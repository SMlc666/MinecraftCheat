#include "Nuker.hpp"
#include "Helper/Block/Block.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/GameType.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
namespace {
std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"range", 10},
    {"spoofCreative", false},
};
}
cheat::Nuker::Nuker() : Module("Nuker", MenuType::WORLD_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("spoofCreative", "伪装创造模式");
    module->getGUI().SliderInt("range", "范围", 1, 20);
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    try {
      int Range = module->getGUI().Get<int>("range");
      bool spoofCreative = module->getGUI().Get<bool>("spoofCreative");
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

      // 遍历范围内的所有方块
      if (spoofCreative)
        player->setPlayerGameType(GameType::Creative);
      for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
          for (int z = startZ; z <= endZ; ++z) {
            glm::ivec3 targetPos(x, y, z);
            if (!Helper::Block::isAirBlock(targetPos))
              player->getGameMode().destroyBlock(targetPos, 1);
          }
        }
      }
      player->setPlayerGameType(GameType::Default);
    } catch (...) {
      return;
    }
  });
}