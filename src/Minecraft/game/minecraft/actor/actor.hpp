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
  std::byte padding324[0x4];
  glm::vec2 *mRot;

public:
  [[nodiscard]] glm::vec3 getPosition() const;
  void setPosition(glm::vec3 pos);
  [[nodiscard]] float getDistance(glm::vec3 pos) const;
  float getDistance(const Actor *actor) const;
  void setRotation(glm::vec2 rot);
  [[nodiscard]] glm::vec2 getRotation() const;
  [[nodiscard]] float getPitch() const;
  [[nodiscard]] float getYaw() const;
  void setPitch(float pitch);
  void setYaw(float yaw);
  [[nodiscard]] int getHealth() const;
}; // namespace class Actor
static_assert(offsetof(Actor, WeakRef_mDimension) == 0x2D8);
static_assert(offsetof(Actor, mPos) == 0x318);
static_assert(offsetof(Actor, mRot) == 808);
static_assert(sizeof(glm::vec3) == 0xC);
static_assert(sizeof(glm::vec2) == 0x8);
static_assert(sizeof(WeakRef<Dimension>) == 16);