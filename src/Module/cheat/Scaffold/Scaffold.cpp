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
static std::pair<bool, uchar> getSupportFace(const BlockPos &pos) {
  ClientInstance *instance = runtimes::getClientInstance();
  auto *region = instance->getRegion();

  Block *downBlock = region->getBlock(pos.x, pos.y - 1, pos.z);
  if (downBlock && downBlock->mBlockLegacy &&
      downBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 1}; // UP face

  Block *eastBlock = region->getBlock(pos.x + 1, pos.y, pos.z);
  if (eastBlock && eastBlock->mBlockLegacy &&
      eastBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 4}; // WEST face

  Block *westBlock = region->getBlock(pos.x - 1, pos.y, pos.z);
  if (westBlock && westBlock->mBlockLegacy &&
      westBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 5}; // EAST face

  Block *southBlock = region->getBlock(pos.x, pos.y, pos.z + 1);
  if (southBlock && southBlock->mBlockLegacy &&
      southBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 2}; // NORTH face

  Block *northBlock = region->getBlock(pos.x, pos.y, pos.z - 1);
  if (northBlock && northBlock->mBlockLegacy &&
      northBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 3}; // SOUTH face

  Block *upBlock = region->getBlock(pos.x, pos.y + 1, pos.z);
  if (upBlock && upBlock->mBlockLegacy &&
      upBlock->mBlockLegacy->getName().find("air") == std::string::npos)
    return {true, 0}; // DOWN face

  return {false, 0};
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

    glm::vec3 playerPos = player->getPosition();
    BlockPos currentPos(static_cast<int>(std::floor(playerPos.x)),
                        static_cast<int>(std::floor(playerPos.y - 1)),
                        static_cast<int>(std::floor(playerPos.z)));

    if (canPlace(currentPos)) {
      auto [supported, face] = getSupportFace(currentPos);
      if (supported) {
        buildScaffold(player, currentPos, face);
        return;
      }
    }

    float yawRad = glm::radians(player->getYaw());
    glm::vec3 frontDir(-sin(yawRad), 0, cos(yawRad));

    auto quantize = [](float f) { return f > 0 ? 1 : (f < 0 ? -1 : 0); };
    BlockPos frontOffset(quantize(frontDir.x), 0, quantize(frontDir.z));
    BlockPos rightOffset(frontOffset.z, 0, -frontOffset.x);
    BlockPos leftOffset(-frontOffset.z, 0, frontOffset.x);
    BlockPos backOffset(-frontOffset.x, 0, -frontOffset.z);

    std::vector<BlockPos> candidates = {currentPos + frontOffset, currentPos + rightOffset,
                                        currentPos + leftOffset, currentPos + backOffset};

    for (const auto &candidate : candidates) {
      if (!canPlace(candidate)) {
        continue;
      }

      auto [supported, face] = getSupportFace(candidate);
      if (supported) {
        buildScaffold(player, candidate, face);
        return;
      }
    }
  });
}
