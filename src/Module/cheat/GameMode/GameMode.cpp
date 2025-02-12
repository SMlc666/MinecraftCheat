#include "GameMode.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"mode", 0},
};
const std::vector<std::string> ModeItems = {
    "Creative",
    "Survival",
    "Adventure",
    "Spectator",
};
} // namespace
cheat::GameMode::GameMode() : Module("GameMode", MenuType::WORLD_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    try {
      int Mode = module->getGUI().Get<int>("mode");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance) {
        return;
      }
      auto player = instance->getLocalPlayer();
      if (!player) {
        return;
      }
      switch (Mode) {
      case 0:
        player->setPlayerGameType(GameType::Creative);
        break;
      case 1:
        player->setPlayerGameType(GameType::Survival);
        break;
      case 2:
        player->setPlayerGameType(GameType::Adventure);
        break;
      case 3:
        player->setPlayerGameType(GameType::Spectator);
        break;
      default:
        g_log_tool.message(LogLevel::WARN, "GameMode", "Invalid game mode: {}", Mode);
        break;
      }
    } catch (...) {
      return;
    }
  });
  setOnDisable([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (!instance) {
      return;
    }
    auto player = instance->getLocalPlayer();
    if (!player) {
      return;
    }
    player->setPlayerGameType(GameType::Default);
  });
  setOnDrawGUI([](Module *module) { module->getGUI().Selectable("mode", "模式", ModeItems); });
}