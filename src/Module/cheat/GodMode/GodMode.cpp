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
};
cheat::GodMode::GodMode() : Module("GodMode", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    if (!module->getGUI().Get<bool>("enabled")) {
      return;
    }
    ClientInstance *client = runtimes::getClientInstance();
    if (!client) {
      return;
    }
    LocalPlayer *player = client->getLocalPlayer();
    if (!player) {
      return;
    }
    player->getGameMode().attack(*player);
  });
}