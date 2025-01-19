#pragma once
#include <cstdint>
#include <functional>
class Player;
class Level {
public:
  uintptr_t **vtable;

public:
  void forEachPlayer(std::function<bool(Player &)> *callback);
};