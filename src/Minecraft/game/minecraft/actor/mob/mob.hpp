#pragma once
#include "game/minecraft/actor/actor.hpp"
class Mob : public Actor {
public:
  void swing();
  void setSpeed(float speed);
  bool isSprinting() const;
};