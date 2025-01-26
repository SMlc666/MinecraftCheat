#include "actor.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "glm/fwd.hpp"
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
float Actor::getDistance(glm::vec3 pos) const {
  glm::vec3 selfPos = this->getPosition();
  return glm::distance(selfPos, pos);
}
float Actor::getDistance(const Actor *actor) const {
  glm::vec3 selfPos = this->getPosition();
  glm::vec3 otherPos = actor->getPosition();
  return glm::distance(selfPos, otherPos);
}
