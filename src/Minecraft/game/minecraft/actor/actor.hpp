#pragma once
#include <cstddef>
#include <memory>
class Dimension;
class Level;
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;
  std::byte padding8[0x2D0];
  std::weak_ptr<Dimension> mDimension;

public:
  [[nodiscard]] Level *getLevel() const;
  [[nodiscard]] Dimension &getDimension() const;
  [[nodiscard]] bool hasDimension() const;
}; // namespace class Actor
static_assert(offsetof(Actor, mDimension) == 0x2D8);