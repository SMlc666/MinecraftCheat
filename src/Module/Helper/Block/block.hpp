#pragma once
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(const glm::ivec3 &pos);
bool isValidPlacementPosition(const glm::ivec3 &pos);
void placeBlock(Player *player, const glm::ivec3 &pos, uchar face);
std::vector<glm::ivec3> getValidPlacementPositions(Player *player, const glm::ivec3 &pos);
bool predictBlock(Player *player, const glm::ivec3 &pos, int distance);
} // namespace Helper::Block