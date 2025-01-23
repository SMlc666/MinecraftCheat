#include "actor.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "signature.hpp"
Level *Actor::getLevel() const {
  using function = Level *(*)(const Actor *);
  auto func = getSign<function>("Actor::getLevel");
  return func(this);
}
Dimension *Actor::getDimension() const {
  std::shared_ptr<Dimension> sharedDimension = mDimension.lock();
  if (sharedDimension) {
    return sharedDimension.get();
  } else {
    return nullptr;
  }
}

bool Actor::hasDimension() const {
  std::shared_ptr<Dimension> sharedDimension = mDimension.lock();
  return sharedDimension != nullptr;
}