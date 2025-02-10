#include "ContainerScreenController.hpp"
ItemStack *ContainerScreenController::_getItemStack(const std::string &collectionName,
                                                    int collectionIndex) {
  using function = ItemStack *(*)(const ContainerScreenController *, const std::string &, int);
  auto func = reinterpret_cast<function>(vtable[62]);
  return func(this, collectionName, collectionIndex);
}