#pragma once
#include "game/common/gamerefs/OwnerPtr.hpp"
#include "game/common/gamerefs/gamerefs_shareptr/GameRefsSharePtr.hpp"
#include <cstddef>
#include <memory>
class Dimension;
class Level;
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;
  std::byte padding8[0x2D0];
  OwnerPtr<SharePtrRefTraits<Dimension>::OwnerStorage> mDimension;

public:
  [[nodiscard]] Level *getLevel() const;
  [[nodiscard]] Dimension *getDimension() const;
  [[nodiscard]] bool hasDimension() const;
}; // namespace class Actor
static_assert(offsetof(Actor, mDimension) == 0x2D8);
static_assert(sizeof(std::weak_ptr<std::shared_ptr<Dimension>>) == 16);