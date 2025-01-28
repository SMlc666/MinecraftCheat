#include "ActorCollision.hpp"
#include "signature.hpp"
bool ActorCollision::isOnGround(const EntityContext &provider) {
  using function = bool (*)(const EntityContext &);
  auto func = getSign<function>("ActorCollision::isOnGround");
  return func(provider);
}