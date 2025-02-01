#include "mob.hpp"
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