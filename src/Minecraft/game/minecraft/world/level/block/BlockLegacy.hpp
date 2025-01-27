#pragma once
#include <cstdint>
#include <string>
class BlockLegacy {
public:
  uintptr_t **vtable;
  std::byte padding8[0x20];
  std::string *name;

public:
  [[nodiscard]] std::string getName() const;
};
static_assert(offsetof(BlockLegacy, name) == 0x28);