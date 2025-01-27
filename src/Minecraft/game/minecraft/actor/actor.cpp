#include "actor.hpp"
#include "glm/fwd.hpp"
#include "gsl/assert"
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
glm::vec2 Actor::getRotation() const {
  if (this->mRot == nullptr) {
    gsl::details::terminate();
  }
  return *this->mRot;
}
void Actor::setRotation(glm::vec2 rot) {
  if (this->mRot == nullptr) {
    gsl::details::terminate();
  }
  *this->mRot = rot;
}
float Actor::getPitch() const {
  return this->getRotation().x;
}
float Actor::getYaw() const {
  return this->getRotation().y;
}
void Actor::setPitch(float pitch) {
  glm::vec2 rot = this->getRotation();
  rot.x = pitch;
  this->setRotation(rot);
}
void Actor::setYaw(float yaw) {
  glm::vec2 rot = this->getRotation();
  rot.y = yaw;
  this->setRotation(rot);
}
