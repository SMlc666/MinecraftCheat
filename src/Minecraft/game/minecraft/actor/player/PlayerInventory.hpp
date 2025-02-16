#pragma once

#include "game/minecraft/world/ContainerID.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
class PlayerInventory {
public:
  ItemStack *getItem(int slot, ContainerID containerId) const;
  uint64 getSelectedSlot() const;
  bool selectSlot(int slot, ContainerID containerId);
};