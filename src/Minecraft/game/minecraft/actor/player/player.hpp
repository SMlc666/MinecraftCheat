#pragma once
#include "game/minecraft/actor/mob/mob.hpp"
class GameMode;
class Player : public Mob {
public:
  [[nodiscard]] GameMode &getGameMode() const;
};