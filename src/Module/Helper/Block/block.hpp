#pragma once
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(const glm::ivec3 &pos);
bool canPlaceBlock(const glm::ivec3 &pos);
void buildBlock(Player *player, const glm::ivec3 &pos, uchar face, bool swing);
std::vector<glm::ivec3> getCanBuildBlocks(Player *player, const glm::ivec3 &pos);
} // namespace Helper::Block