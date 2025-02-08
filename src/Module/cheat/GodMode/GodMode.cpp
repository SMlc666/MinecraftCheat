#include "GodMode.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "game/minecraft/network/Packet/Packets/PlayerAuthInputPacket.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <chrono>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"Delay", 100},
    {"Mode", 0},
};
static const std::vector<std::string> ModeItems = {"attackSelf", "setPosition"};
static std::chrono::steady_clock::time_point m_lastGodModeTime{};
cheat::GodMode::GodMode() : Module("GodMode", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderInt("Delay", "间隔", 1, 1000);
    module->getGUI().Selectable("Mode", "模式", ModeItems);
  });
  setOnTick([](Module *module) {
    try {
      auto now = std::chrono::steady_clock::now();
      auto Delay = module->getGUI().Get<int>("Delay");
      if (now - m_lastGodModeTime < std::chrono::milliseconds(Delay)) {
        return;
      }
      int Mode{};
      Mode = module->getGUI().Get<int>("Mode");
      if (Mode != 0) {
        return;
      }
      ClientInstance *client = runtimes::getClientInstance();
      if (client == nullptr) {
        return;
      }
      LocalPlayer *player = client->getLocalPlayer();
      if (player == nullptr) {
        return;
      }
      player->getGameMode().attack(*player);
      m_lastGodModeTime = now;
    } catch (...) {
      return;
    }
  });
  setOnSendPacket([](Module *module, Packet *packet) {
    try {
      auto now = std::chrono::steady_clock::now();
      auto Delay = module->getGUI().Get<int>("Delay");
      if (now - m_lastGodModeTime < std::chrono::milliseconds(Delay)) {
        return true;
      }
      if (packet->getName() == "MovePlayerPacket") {
        auto movePacket = static_cast<MovePlayerPacket *>(packet);
        movePacket->mPos.y += 500.0F;
        m_lastGodModeTime = now;
      } else if (packet->getName() == "PlayerAuthInputPacket") {
        auto authPacket = static_cast<PlayerAuthInputPacket *>(packet);
        authPacket->mPos->y += 500.0F;
        m_lastGodModeTime = now;
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}