#include "GodMode.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"Mode", 0},
};
static const std::vector<std::string> ModeItems = {"attackSelf", "setPosition"};
cheat::GodMode::GodMode() : Module("GodMode", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().Selectable("Mode", "模式", ModeItems); });
  setOnTick([](Module *module) {
    int Mode{};
    try {
      Mode = module->getGUI().Get<int>("Mode");
    } catch (...) {
      return;
    }
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
  });
  setOnSendPacket([](Module *module, Packet *packet) {

  });
}