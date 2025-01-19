#include "Level.hpp"
void Level::forEachPlayer(std::function<bool(Player &)> *callback) {
  using function = void (*)(Level *, std::function<bool(Player &)> *);
  auto func = reinterpret_cast<function>(this->vtable[252]);
  return func(this, callback);
}