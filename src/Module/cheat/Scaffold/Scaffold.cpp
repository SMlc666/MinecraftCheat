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
static bool canPlace(glm::vec3 pos) {
  Block *block = runtimes::getClientInstance()->getRegion()->getBlock(
      static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
  if (block == nullptr) {
    return false;
  }
  BlockLegacy *blockLegacy = block->mBlockLegacy;
  if (blockLegacy == nullptr) {
    return false;
  }
  std::string name = blockLegacy->getName();
  return !(name.find("air") == std::string::npos);
}
static void buildBlock(LocalPlayer *player, BlockPos pos, uchar face) {
  bool swing = g_md->getGUI().Get<bool>("swing");
  GameMode *gameMode = &player->getGameMode();
  if (gameMode == nullptr) {
    return;
  }
  gameMode->buildBlock(pos, face);
  if (swing) {
    player->swing();
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
    auto *player = instance->getLocalPlayer();
    if (player == nullptr) {
      return;
    }
    ItemStack *item = player->getSelectedItem();
    if (item == nullptr) {
      return;
    }
    if (!item->isBlock()) {
      return;
    }
    glm::vec3 playerPos = player->getPosition();
    glm::vec3 motion = player->getMotion();
    glm::vec3 targetPos = playerPos;
    targetPos.y -= 1.0f;
    if (std::abs(motion.x) > 0.01f || std::abs(motion.z) > 0.01f) {
      targetPos.x += (motion.x > 0 ? 1.0f : (motion.x < 0 ? -1.0f : 0.0f));
      targetPos.z += (motion.z > 0 ? 1.0f : (motion.z < 0 ? -1.0f : 0.0f));
    }
    if (canPlace(targetPos)) {
      BlockPos placePos(static_cast<int>(targetPos.x), static_cast<int>(targetPos.y),
                        static_cast<int>(targetPos.z));
      uchar face = 1;
      glm::vec3 diff = playerPos - targetPos;
      if (std::abs(diff.y) > std::abs(diff.x) && std::abs(diff.y) > std::abs(diff.z)) {
        face = (diff.y > 0) ? 0 : 1;
      } else if (std::abs(diff.x) > std::abs(diff.z)) {
        face = (diff.x > 0) ? 4 : 5;
      } else {
        face = (diff.z > 0) ? 2 : 3;
      }
      buildBlock(player, placePos, face);
    }
    if (motion.y > 0) {
      glm::vec3 upTargetPos = playerPos;
      upTargetPos.y += 2.0f;

      if (canPlace(upTargetPos)) {
        BlockPos upPlacePos(static_cast<int>(upTargetPos.x), static_cast<int>(upTargetPos.y),
                            static_cast<int>(upTargetPos.z));
        buildBlock(player, upPlacePos, 0);
      }
    }
  });
}