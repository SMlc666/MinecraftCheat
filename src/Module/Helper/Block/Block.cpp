#include "Block.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
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
