#include "Block.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "glm/fwd.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
bool Helper::Block::isAirBlock(glm::ivec3 pos) {
  auto *client = runtimes::getClientInstance();
  if (client == nullptr) {
    return false;
  }
  BlockSource *region = client->getRegion();
  if (region == nullptr) {
    return false;
  }
  ::Block *block = region->getBlock(pos);
  if (block == nullptr) {
    return false;
  }
  BlockLegacy *legacy = block->mBlockLegacy;
  if (legacy == nullptr) {
    return false;
  }
  return legacy->getName().find("air") != std::string::npos;
}
bool Helper::Block::canPlaceBlock(glm::ivec3 pos) {
  static const std::vector<glm::ivec3> offsets = {{0, 1, 0},  {0, -1, 0}, {1, 0, 0},
                                                  {-1, 0, 0}, {0, 0, 1},  {0, 0, -1}};
  bool canPlace = true;
  auto *client = runtimes::getClientInstance();
  if (client == nullptr) {
    return false;
  }
  ::Block *block = client->getRegion()->getBlock(pos);
  if (block == nullptr) {
    return false;
  }
  if (std::ranges::any_of(offsets, [&](const auto &offset) {
        auto neighborPos = pos + offset;
        return !isAirBlock(neighborPos);
      })) {
    return true;
  }
  return false;
}
bool Helper::Block::tryScaffold(LocalPlayer *player, glm::vec3 blockBelow) {
  blockBelow = glm::floor(blockBelow);
  BlockPos block(blockBelow);
  static std::vector<BlockPos> checklist = {
      BlockPos(0, -1, 0), // 下方
      BlockPos(0, 1, 0),  // 上方
      BlockPos(0, 0, -1), // 后方
      BlockPos(0, 0, 1),  // 前方
      BlockPos(-1, 0, 0), // 左侧
      BlockPos(1, 0, 0)   // 右侧
  };
  for (const auto &offset : checklist) {
    BlockPos neighbour = block - offset;
    if (!isAirBlock(neighbour)) {
      player->getGameMode().buildBlock(
          neighbour,
          std::distance(checklist.begin(), std::find(checklist.begin(), checklist.end(), offset)));
      return true;
    }
  }
  return false;
}