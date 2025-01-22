#include "Level.hpp"
void Level::forEachPlayer(std::function<bool(Player &)> *callback) {
  using function = void (*)(Level *, std::function<bool(Player &)> *);
  auto func = reinterpret_cast<function *>((reinterpret_cast<char *>(this) + 0x7E0LL));
  return (*func)(this, callback);
}