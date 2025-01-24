#pragma once
#include "game/common/gamerefs/WeakRef.hpp"
#include "game/minecraft/world/level/BlockSourceListener.hpp"
#include <cstddef>
#include <memory>
class Dimension;
class Level;
class BlockSource;
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;
  std::byte padding8[0x2D0];
  BlockSource &mRegion;

public:
  [[nodiscard]] Level *getLevel() const;
  [[nodiscard]] Dimension *getDimension() const;
  [[nodiscard]] bool hasDimension() const;
}; // namespace class Actor
static_assert(offsetof(Actor, mRegion) == 0x2D8);
static_assert(sizeof(WeakRef<Dimension>) == 16);