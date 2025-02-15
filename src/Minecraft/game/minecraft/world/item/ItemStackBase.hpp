#pragma once
#include <cstdint>
#include <string>
class ItemStackBase {
public:
  uintptr_t **vtable;

public:
  [[nodiscard]] bool isNull() const;
  [[nodiscard]] bool isBlock() const;
  std::string getRawNameId() const;
};