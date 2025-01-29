#include "Scaffold.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockPos.hpp"
#include "glm/fwd.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"swing", false}};
static const std::unordered_map<glm::ivec3, bool> canPlaceMap = {
    //以我的世界北方向为准
    {glm::ivec3(0, -1, 0), true}, //下方
    {glm::ivec3(0, 1, 0), true},  //上方
    {glm::ivec3(0, 0, -1), true}, //前方
    {glm::ivec3(0, 0, 1), true},  //后方
    {glm::ivec3(-1, 0, 0), true}, //左侧
    {glm::ivec3(1, 0, 0), true},  //右侧
};
static bool isAirBlock(const glm::vec3 &pos) {
  Block *block = runtimes::getClientInstance()->getRegion()->getBlock(
      static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
  if ((block == nullptr) || (block->mBlockLegacy == nullptr)) {
    return false;
  }
  return block->mBlockLegacy->getName().find("air") != std::string::npos;
}
static bool canPlace(const glm::vec3 &pos) {
  bool can = false;
  if (!isAirBlock(pos)) {
    return false;
  }
  glm::ivec3 blockPos = pos;
  for (auto const &pair : canPlaceMap) {
    blockPos = pos + glm::vec3(pair.first);
    if (!isAirBlock(glm::vec3(blockPos))) {
      can = true;
      break;
    }
  }
  return can;
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
