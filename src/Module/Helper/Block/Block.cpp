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
bool Helper::Block::tryScaffold(LocalPlayer *player, glm::vec3 blockBelow, bool strict) {
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
  bool foundCandidate = false;
  int i = 0;
  for (auto current : checklist) {
    BlockPos calc = block - current;
    if (!isAirBlock(calc) && (strict || canPlaceBlock(calc))) {
      foundCandidate = true;
      block = calc;
      break;
    }
    i++;
  }
  if (foundCandidate) {
    player->getGameMode().buildBlock(block, i);
    return true;
  }
  return false;
}
void Helper::Block::extendBlock(const glm::vec3 &velocity, glm::vec3 &blockBelow, int extend) {
  blockBelow.x += velocity.x * extend;
  blockBelow.z += velocity.z * extend;
}
void Helper::Block::adjustYCoordinate(glm::vec3 &blockBelow, const glm::vec3 &blockBelowReal,
                                      float &YCoord) {
  blockBelow.y = YCoord;
  if (blockBelowReal.y < YCoord) {
    YCoord = blockBelowReal.y;
  }
}
glm::vec3 Helper::Block::getBlockBelow(Player *player, float yOffset) {
  glm::vec3 blockBelow = player->getPosition();
  blockBelow.y += yOffset;
  return blockBelow;
}