#include "Teams.hpp"
#include "Helper/Target/Target.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"range", 10.0F},
};
static std::vector<std::string> TeamNames;
static MemTool::Hook Target_ProcessPlayer;
static bool Target_ProcessPlayer_(Player &player, LocalPlayer *localPlayer, bool antibot,
                                  float range, float fov, std::vector<Player *> &playerList) {
  std::string name = player.getName();
  for (const auto &teamName : TeamNames) {
    if (name == teamName) {
      return true;
    }
  }
  auto ret = Target_ProcessPlayer.call<bool>(player, localPlayer, antibot, range, fov, playerList);
  return ret;
}
cheat::Teams::Teams() : Module("Teams", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    try {
      float range = module->getGUI().Get<float>("range");
      module->getGUI().Set("enabled", false);
      g_log_tool.message(LogLevel::INFO, "Teams", "Teams module enabled");
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance == nullptr) {
        g_log_tool.message(LogLevel::ERROR, "Teams", "Failed to get client instance");
        return;
      }
      LocalPlayer *player = instance->getLocalPlayer();
      if (player == nullptr) {
        g_log_tool.message(LogLevel::ERROR, "Teams", "Failed to get local player");
        return;
      }
      Dimension *dimension = player->mDimension;
      if (dimension == nullptr) {
        g_log_tool.message(LogLevel::ERROR, "Teams", "Failed to get dimension");
        return;
      }
      TeamNames.clear();
      glm::vec3 pos = player->getPosition();
      dimension->forEachPlayer([&](Player &player) {
        if (player.getDistance(pos) <= range) {
          std::string name = player.getName();
          if (!name.empty()) {
            g_log_tool.message(LogLevel::INFO, "Teams", "Found player: {}", name);
            TeamNames.push_back(name);
          }
        }
        return true;
      });
    } catch (...) {
      return;
    }
  });
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().SliderFloat("range", "范围", 1.0F, 30.0F); });
  setOnLoad([](Module *module) {
    Target_ProcessPlayer =
        MemTool::Hook(&Helper::Target::ProcessPlayer,
                      reinterpret_cast<void *>(&Target_ProcessPlayer_), nullptr, false);
  });
}