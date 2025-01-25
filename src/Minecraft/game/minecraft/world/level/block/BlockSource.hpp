#pragma once
#include "game/minecraft/actor/actor.hpp"
#include <cstdint>
class BlockSource {
public:
  uintptr_t **vtable;
  std::byte padding8[48];
  Dimension *mDimension;
  std::byte padding40[248];
};
static_assert(offsetof(BlockSource, mDimension) == 0x38);
static_assert(sizeof(BlockSource) == 0x138);