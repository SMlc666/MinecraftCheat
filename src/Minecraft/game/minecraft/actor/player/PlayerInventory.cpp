#include "PlayerInventory.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "signature.hpp"

ItemStack *PlayerInventory::getItem(int slot, ContainerID containerId) const {
  using function = ItemStack *(*)(const PlayerInventory *, int, ContainerID);
  auto func = getSign<function>("PlayerInventory::getItem");
  return func(this, slot, containerId);
}
bool PlayerInventory::selectSlot(int slot, ContainerID containerId) {
  using function = bool (*)(PlayerInventory *, int, ContainerID);
  auto func = getSign<function>("PlayerInventory::selectSlot");
  return func(this, slot, containerId);
}
int PlayerInventory::getSelectedSlot() const {
  using function = int (*)(const PlayerInventory *);
  auto func = getSign<function>("PlayerInventory::getSelectedSlot");
  return func(this);
}