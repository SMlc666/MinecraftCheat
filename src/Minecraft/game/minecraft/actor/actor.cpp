#include "actor.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "gsl/assert"
#include "signature.hpp"
glm::vec3 Actor::getPosition() const {
  if (this->mPos == nullptr) {
    gsl::details::terminate();
  }
  return *this->mPos;
}
void Actor::setPosition(glm::vec3 pos) {
  if (this->mPos == nullptr) {
    gsl::details::terminate();
  }
  *this->mPos = pos;
}