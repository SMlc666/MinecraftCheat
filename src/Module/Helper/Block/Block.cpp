#include "Block.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtimes/runtimes.hpp"
#include <string>
static std::vector<BlockPos> checklist = {
    BlockPos(0, -1, 0), // 下方
    BlockPos(0, 1, 0),  // 上方
    BlockPos(0, 0, -1), // 后方
    BlockPos(0, 0, 1),  // 前方
    BlockPos(-1, 0, 0), // 左侧
    BlockPos(1, 0, 0)   // 右侧
};
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
bool Helper::Block::tryClutchScaffold(LocalPlayer *player, BlockSource *region,
                                      glm::vec3 blockBelow, bool strict) {
  glm::vec3 vel = player->getMotion();
  vel = glm::normalize(vel);
  blockBelow = glm::floor(blockBelow);

  static std::vector<glm::ivec3> checkBlocks;
  if (checkBlocks.empty()) {
    for (int y = -4; y <= 4; y++) {
      for (int x = -4; x <= 4; x++) {
        for (int z = -4; z <= 4; z++) {
          checkBlocks.push_back(glm::ivec3(x, y, z));
        }
      }
    }
    std::sort(checkBlocks.begin(), checkBlocks.end(), [](glm::ivec3 first, glm::ivec3 last) {
      return sqrtf((float)(first.x * first.x) + (float)(first.y * first.y) +
                   (float)(first.z * first.z)) <
             sqrtf((float)(last.x * last.x) + (float)(last.y * last.y) + (float)(last.z * last.z));
    });
  }

  for (const glm::ivec3 &blockOffset : checkBlocks) {
    glm::ivec3 currentBlock = glm::ivec3(blockBelow) + blockOffset;
    if (Helper::Block::isAirBlock(currentBlock)) {
      if (tryScaffold(player, currentBlock, false)) {
        return true;
      }
    }
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
  blockBelow.y -= yOffset;
  return blockBelow;
}
glm::vec3 Helper::Block::getNextBlock(Player *player, const glm::vec3 &velocity,
                                      const glm::vec3 &blockBelow) {
  glm::vec3 nextBlock = blockBelow;
  if (abs(velocity.x) > abs(velocity.z)) {
    nextBlock.x += (velocity.x > 0 ? 1 : (velocity.x < 0 ? -1 : 0));
  } else {
    nextBlock.z += (velocity.z > 0 ? 1 : (velocity.z < 0 ? -1 : 0));
  }
  return nextBlock;
}
bool Helper::Block::blockNameHas(glm::ivec3 pos, const std::string &name) {
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
  return legacy->getName().find(name) != std::string::npos;
}