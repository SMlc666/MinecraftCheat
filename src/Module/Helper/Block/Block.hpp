#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(glm::ivec3 pos);
bool canPlaceBlock(glm::ivec3 pos);
bool tryScaffold(LocalPlayer *player, glm::vec3 blockBelow);
} // namespace Helper::Block