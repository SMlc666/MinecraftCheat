#include "Gapple.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>

namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"health", 10},
};
}
cheat::Gapple::Gapple() : Module("Gapple", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *) {});
  setOnDisable([](Module *) {});
  setOnDrawGUI([](Module *module) { module->getGUI().SliderInt("health", "血量", 1, 20); });
  setOnTick([](Module *module) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      auto player = instance->getLocalPlayer();
      if (!player)
        return;
      int CfgHealth = module->getGUI().Get<int>("health");
      int health = player->getHealth();
      if (health > CfgHealth)
        return;
      auto inv = player->getSupplies();
      for (int i = 0; i < 9; i++) {
        ItemStack *items = inv.getItem(i, ContainerID::Inventory);
        if (items->getRawNameId().find("golden_apple") != std::string::npos) {
          GameMode *gamemode = &player->getGameMode();
          int idx = inv.getSelectedSlot();
          inv.selectSlot(i, ContainerID::Inventory);
          gamemode->useItem(*items);
          inv.selectSlot(idx, ContainerID::Inventory);
          return;
        }
      }
    } catch (...) {
      return;
    }
  });
}