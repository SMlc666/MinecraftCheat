#pragma once
#include "glm/fwd.hpp"
namespace Helper::Block {
bool isAirBlock(glm::ivec3 pos);
bool canPlaceBlock(glm::ivec3 pos);
} // namespace Helper::Block