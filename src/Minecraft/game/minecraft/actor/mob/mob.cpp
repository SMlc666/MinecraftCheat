#include "mob.hpp"
#include "signature.hpp"
void Mob::swing() {
  using function = void (*)(const Mob *);
  auto func = reinterpret_cast<function>(this->vtable[144]);
  return func(this);
}
void Mob::setSpeed(float speed) {
  using function = void (*)(Mob *, float);
  auto func = reinterpret_cast<function>(this->vtable[189]);
  return func(this, speed);
}
bool Mob::isSprinting() const {
  using function = bool (*)(const Mob *);
  auto func = getSign<function>("Mob::isSprinting");
  return func(this);
}