#pragma once
#include "game/minecraft/world/item/ItemStack.hpp"
#include <cstdint>
#include <string>
class ContainerScreenController {
public:
  uintptr_t **vtable;

public:
  ItemStack *_getItemStack(const std::string &collectionName, int collectionIndex);
};