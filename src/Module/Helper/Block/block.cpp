#include "block.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "runtimes/runtimes.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockPos.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
static const std::unordered_map<glm::ivec3, bool> canPlaceMap = {
    //以我的世界北方向为准
    {glm::ivec3(0, -1, 0), true}, //下方
    {glm::ivec3(0, 1, 0), true},  //上方
    {glm::ivec3(0, 0, -1), true}, //前方
    {glm::ivec3(0, 0, 1), true},  //后方
    {glm::ivec3(-1, 0, 0), true}, //左侧
    {glm::ivec3(1, 0, 0), true},  //右侧
};
bool Helper::Block::isAirBlock(const glm::ivec3 &pos) {
  ::Block *block = runtimes::getClientInstance()->getRegion()->getBlock(
      static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
  if ((block == nullptr) || (block->mBlockLegacy == nullptr)) {
    return false;
  }
  return block->mBlockLegacy->getName().find("air") != std::string::npos;
}
bool Helper::Block::canPlaceBlock(const glm::ivec3 &pos) {
  bool can = false;
  if (!isAirBlock(pos)) {
    return false;
  }
  glm::ivec3 blockPos = pos;
  for (auto const &pair : canPlaceMap) {
    blockPos = pos + glm::ivec3(pair.first);
    if (!isAirBlock(glm::ivec3(blockPos))) {
      can = true;
      break;
    }
  }
  return can;
}
void Helper::Block::buildBlock(Player *player, const BlockPos &pos, uchar face, bool swing) {
  if (auto *gameMode = &player->getGameMode(); gameMode != nullptr) {
    gameMode->buildBlock(pos, face);
    if (swing) {
      player->swing();
    }
  }
}