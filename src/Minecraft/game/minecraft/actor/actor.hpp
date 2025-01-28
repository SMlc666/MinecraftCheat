#pragma once
#include "game/common/gamerefs/WeakRef.hpp"
#include "game/minecraft/world/entity/EntityContext.hpp"
#include "game/minecraft/world/level/BlockSourceListener.hpp"
#include <cstddef>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>
class Dimension;
class Level;
class BlockSource;
struct PosMotion {
  glm::vec3 pos;
  glm::vec3 paddingpos;
  glm::vec3 motion;
};
static_assert(offsetof(PosMotion, motion) == 24);
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;
  EntityContext mEntityContext;
  std::byte padding8[0x2B0];
  Dimension *mDimension;
  WeakRef<Dimension> WeakRef_mDimension;
  std::byte padding2E8[0x2C];
  std::byte padding314[0x4];
  PosMotion *mPosMotion;
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
  [[nodiscard]] bool isAlive() const;
  [[nodiscard]] glm::vec3 getMotion() const;
  void setMotion(glm::vec3 motion);
}; // namespace class Actor
static_assert(sizeof(EntityContext) == 24);
static_assert(offsetof(Actor, WeakRef_mDimension) == 0x2D8);
static_assert(offsetof(Actor, mPosMotion) == 0x318);
static_assert(offsetof(Actor, mRot) == 808);
static_assert(sizeof(glm::vec3) == 0xC);
static_assert(sizeof(glm::vec2) == 0x8);
static_assert(sizeof(WeakRef<Dimension>) == 16);