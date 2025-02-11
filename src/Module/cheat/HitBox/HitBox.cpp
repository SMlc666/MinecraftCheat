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
}
cheat::HitBox::HitBox() : Module("HitBox", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().SliderFloat("Scale", "缩放", 0.1F, 3.0F); });
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
        player->mAABB->Scale(Scale);
      }
    } catch (...) {
      return;
    }
  });
};
