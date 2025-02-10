#include "Criticals.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"Mode", 0},
};
const std::vector<std::string> ModeItems = {"BJD", "Jump"};
MemTool::Hook GameMode_attack_;
Module *g_md = nullptr;
bool GameMode_attack(GameMode *self, Actor &entity) {
  try {
    if (g_md != nullptr && g_md->getGUI().Get<bool>("enabled")) {
      int Mode = g_md->getGUI().Get<int>("Mode");
      if (Mode == 1) {
        ClientInstance *instance = runtimes::getClientInstance();
        if (instance != nullptr) {
          auto *player = instance->getLocalPlayer();
          if (player != nullptr) {
            if (&player->getGameMode() == self &&
                ActorCollision::isOnGround(player->mEntityContext)) {
              player->jumpFromGround();
            }
          }
        }
      }
    }
  } catch (...) {
  }
  auto ret = GameMode_attack_.call<bool>(self, entity);
  return ret;
}
float height = 1.2f;
}; // namespace

cheat::Criticals::Criticals() : Module("Criticals", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().Selectable("Mode", "模式", ModeItems); });
  setOnLoad([](Module *module) {
    g_md = module;
    void *gamemode = getSign<void *>("GameMode::attack");
    GameMode_attack_ =
        MemTool::Hook(gamemode, reinterpret_cast<void *>(GameMode_attack), nullptr, false);
  });
  setOnSendPacket([](Module *module, Packet *packet) {
    try {
      int mode = g_md->getGUI().Get<int>("Mode");
      if (mode == 0) {
        if (packet->getName() == "MovePlayerPacket") {
          auto *move = reinterpret_cast<MovePlayerPacket *>(packet);
          move->mOnGround = false;
          move->mPos = glm::vec3(move->mPos.x, move->mPos.y + height, move->mPos.z);
          height -= 0.1f;
          if (height <= 0.3f)
            height = 1.2f;
          return true;
        }
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}