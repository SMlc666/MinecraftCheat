#include "mob.hpp"
void Mob::swing() {
  using function = void (*)(const Mob *);
  auto func = reinterpret_cast<function>(this->vtable[30]);
  return func(this);
}