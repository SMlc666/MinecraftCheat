#pragma once
#include "game/minecraft/actor/actor.hpp"
class Mob : public Actor {
public:
  std::byte paddingE68[0xA9A];

public:
  void swing();
  void setSpeed(float speed);
  bool isSprinting() const;
  void jumpFromGround();
};
static_assert(sizeof(Mob) == 0x11CE);