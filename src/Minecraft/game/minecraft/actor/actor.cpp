#include "actor.hpp"
#include "glm/fwd.hpp"
#include "gsl/assert"
#include "signature.hpp"
glm::vec3 Actor::getPosition() const {
  if (this->mPosMotion == nullptr) {
    gsl::details::terminate();
  }
  return this->mPosMotion->pos;
}
void Actor::setPosition(glm::vec3 pos) {
  using function = void (*)(Actor *, glm::vec3 *);
  auto func = getSign<function>("Actor::setPos");
  func(this, &pos);
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
int Actor::getHealth() const {
  using function = int (*)(const Actor *);
  auto func = getSign<function>("Actor::getHealth");
  return func(this);
}
bool Actor::isAlive() const {
  using function = bool (*)(const Actor *);
  auto func = reinterpret_cast<function>(vtable[61]);
  return func(this);
}
glm::vec3 Actor::getMotion() const {
  if (this->mPosMotion == nullptr) {
    gsl::details::terminate();
  }
  return this->mPosMotion->motion;
}
void Actor::setMotion(glm::vec3 motion) {
  if (this->mPosMotion == nullptr) {
    gsl::details::terminate();
  }
  this->mPosMotion->motion = motion;
}
