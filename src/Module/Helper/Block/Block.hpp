#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(glm::ivec3 pos);
bool canPlaceBlock(glm::ivec3 pos);
bool tryScaffold(LocalPlayer *player, glm::vec3 blockBelow, bool strict = false);
void extendBlock(const glm::vec3 &velocity, glm::vec3 &blockBelow, int extend);
void adjustYCoordinate(glm::vec3 &blockBelow, const glm::vec3 &blockBelowReal, float &YCoord);
glm::vec3 getBlockBelow(Player *player, float yOffset);
glm::vec3 getNextBlock(Player *player, const glm::vec3 &velocity, const glm::vec3 &blockBelow);
bool tryClutchScaffold(LocalPlayer *player, BlockSource *region, glm::vec3 blockBelow,
                       bool strict = false);
bool blockNameHas(glm::ivec3 pos, const std::string &name);
} // namespace Helper::Block