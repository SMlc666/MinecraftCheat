#pragma once
#include "game/common/gamerefs/WeakRef.hpp"
#include "game/minecraft/world/level/BlockSourceListener.hpp"
#include <cstddef>
#include "glm/glm.hpp"
#include <memory>
class Dimension;
class Level;
class BlockSource;
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;
  std::byte padding8[0x2C8];
  Dimension *mDimension;
  WeakRef<Dimension> WeakRef_mDimension;
  std::byte padding2E8[0x2C];
  std::byte padding314[0x4];
  glm::vec3 *mPos;

public:
}; // namespace class Actor
static_assert(offsetof(Actor, WeakRef_mDimension) == 0x2D8);
static_assert(offsetof(Actor, mPos) == 0x318);
static_assert(sizeof(glm::vec3) == 0xC);
static_assert(sizeof(WeakRef<Dimension>) == 16);