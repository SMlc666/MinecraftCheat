#include "block.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "glm/fwd.hpp"
#include "runtimes/runtimes.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockPos.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include <vector>
static const std::vector<glm::ivec3> neighbours = {
    glm::ivec3(0, -1, 0), glm::ivec3(0, 1, 0),  glm::ivec3(0, 0, -1),
    glm::ivec3(0, 0, 1),  glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0),
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
bool Helper::Block::buildBlock(glm::ivec3 pos, GameMode *gameMode) {
  if (gameMode == nullptr) {
    return false;
  }
  for (int face = 0; face < neighbours.size(); face++) {
    auto offest = glm::ivec3(pos - neighbours[face]);
    if (!isAirBlock(offest)) {
      gameMode->buildBlock(offest, face);
      return true;
    }
  }
  return false;
}
void Helper::Block::predictBlock(glm::ivec3 pos, GameMode *gameMode, float distance) {
  if (gameMode == nullptr) {
    return;
  }
  std::vector<glm::ivec3> blocks;
  for (int y = -distance; y <= 0; y++) {
    for (int x = -distance; x <= 0; x++) {
      for (int z = -distance; z <= 0; z++) {
        blocks.emplace_back(x, y, z);
      }
    }
  }
  std::sort(blocks.begin(), blocks.end(),
            [](glm::ivec3 start, glm::ivec3 end) { return glm::length(start) < glm::length(end); });
  for (const glm::ivec3 &offset : blocks) {
    glm::ivec3 currentBlock = pos + offset;
    if (buildBlock(currentBlock, gameMode)) {
      return;
    }
  }
}