#include "Scaffold.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockPos.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"swing", false}};
static bool canPlace(const glm::vec3 &pos) {
  Block *block = runtimes::getClientInstance()->getRegion()->getBlock(
      static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
  if ((block == nullptr) || (block->mBlockLegacy == nullptr)) {
    return false;
  }
  return block->mBlockLegacy->getName().find("air") != std::string::npos;
}
static void buildScaffold(LocalPlayer *player, const BlockPos &pos, uchar face) {
  if (auto *gameMode = &player->getGameMode(); gameMode != nullptr) {
    gameMode->buildBlock(pos, face);
    if (g_md->getGUI().Get<bool>("swing")) {
      player->swing();
    }
  }
}

cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().CheckBox("swing", "挥手"); });
  setOnRender([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }
    LocalPlayer *player = instance->getLocalPlayer();
    if ((player == nullptr) || player->getMotion().y <= 0) {
      return;
    }

    ItemStack *item = player->getSelectedItem();
    if ((item == nullptr) || !item->isBlock()) {
      return;
    }
  });
}
