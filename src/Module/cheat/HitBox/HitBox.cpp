#include "HitBox.hpp"
#include "Helper/Target/Target.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <iosfwd>
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"Range", 6.0F},
    {"AntiBot", true},  {"Scale", 1.1F},     {"Fov", 360.0F},
};
std::unordered_map<Player *, AABB> originalAABBs;
} // namespace

cheat::HitBox::HitBox() : Module("HitBox", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {
    for (auto &[player, originalAABB] : originalAABBs) {
      if (player && player->mAABB) { 
        *player->mAABB = originalAABB; 
      }
    }
    originalAABBs.clear();
  });
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("Scale", "缩放", 0.1F, 3.0F);
    module->getGUI().CheckBox("AntiBot", "反机器人");
    module->getGUI().SliderFloat("Range", "范围", 1.0F, 10.0F);
    module->getGUI().SliderFloat("Fov", "视角", 0.0F, 360.0F);
  });
  setOnTick([](Module *module) {
    try {
      bool AntiBot = module->getGUI().Get<bool>("AntiBot");
      float Range = module->getGUI().Get<float>("Range");
      float Fov = module->getGUI().Get<float>("Fov");
      float Scale = module->getGUI().Get<float>("Scale");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      auto *mLocalPlayer = instance->getLocalPlayer();
      if (!mLocalPlayer)
        return;
      Dimension *dimension = mLocalPlayer->mDimension;
      if (!dimension)
        return;
      std::vector<Player *> players;
      dimension->forEachPlayer([&](Player &player) {
        return Helper::Target::ProcessPlayer(player, mLocalPlayer, AntiBot, Range, Fov, players);
      });
      for (auto *player : players) {
        if (originalAABBs.find(player) == originalAABBs.end()) {
          originalAABBs[player] = *player->mAABB;
        }
        *player->mAABB = originalAABBs[player];
        player->mAABB->Scale(Scale);
      }
      for (auto it = originalAABBs.begin(); it != originalAABBs.end();) {
        if (std::find(players.begin(), players.end(), it->first) == players.end()) {
          it = originalAABBs.erase(it);
        } else {
          ++it;
        }
      }
    } catch (...) {
      return;
    }
  });
};
