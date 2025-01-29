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

    // 修改后的移动方向处理逻辑
    float absX = std::abs(motion.x);
    float absZ = std::abs(motion.z);
    if (absX > 0.01f || absZ > 0.01f) {
      // 只处理主要移动方向
      if (absX > absZ) {
        targetPos.x += (motion.x > 0 ? 1.0f : -1.0f);
      } else {
        targetPos.z += (motion.z > 0 ? 1.0f : -1.0f);
      }
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
      buildScaffold(player, placePos, face);
    }
    if (motion.y > 0) {
      glm::vec3 upTargetPos = playerPos;
      upTargetPos.y += 2.0f;

      if (canPlace(upTargetPos)) {
        BlockPos upPlacePos(static_cast<int>(upTargetPos.x), static_cast<int>(upTargetPos.y),
                            static_cast<int>(upTargetPos.z));
        buildScaffold(player, upPlacePos, 0);
      }
    }
  });
}