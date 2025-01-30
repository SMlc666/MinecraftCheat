#pragma once
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(glm::ivec3 pos);
bool buildBlock(glm::ivec3 pos, GameMode *gameMode);
void predictBlock(glm::ivec3 pos, GameMode *gameMode, float distance);
} // namespace Helper::Block