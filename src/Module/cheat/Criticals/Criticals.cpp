#include "Criticals.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "cheat/Seckill/Seckill.hpp"
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
float height = 1.2f;
}; // namespace

cheat::Criticals::Criticals() : Module("Criticals", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().Selectable("Mode", "模式", ModeItems); });

  setOnSendPacket([](Module *module, Packet *packet) {
    try {
      int mode = module->getGUI().Get<int>("Mode");
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
  setOnAttack([](Module *module, MemTool::Hook *mem, Actor *target) {
    try {
      int mode = module->getGUI().Get<int>("Mode");
      if (mode == 1) {
        ClientInstance *instance = runtimes::getClientInstance();
        if (!instance) {
          return;
        }
        auto *player = instance->getLocalPlayer();
        if (!player) {
          return;
        }
        if (ActorCollision::isOnGround(player->mEntityContext)) {
          player->jumpFromGround();
        }
      }
    } catch (...) {
      return;
    }
  });
}