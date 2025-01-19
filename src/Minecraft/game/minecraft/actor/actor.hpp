#pragma once
class Dimension;
class Level;
#include <cstdint>
class Actor {
public:
  uintptr_t **vtable;

public:
  [[nodiscard]] Level *getLevel() const;
  [[nodiscard]] Dimension *getDimension() const;
}; // namespace class Actor