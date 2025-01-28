#pragma once
#include <cstdint>
class ItemStackBase {
public:
  uintptr_t **vtable;

public:
  [[nodiscard]] bool isNull() const;
  [[nodiscard]] bool isBlock() const;
};