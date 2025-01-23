#include "actor.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "signature.hpp"
Level *Actor::getLevel() const {
  using function = Level *(*)(const Actor *);
  auto func = getSign<function>("Actor::getLevel");
  return func(this);
}
Dimension *Actor::getDimension() const {
  return mDimension->mValue.get();
}

bool Actor::hasDimension() const {
  return mDimension->mValue != nullptr;
}