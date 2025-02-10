#pragma once
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
bool GameMode_attack(GameMode *self, Actor *entity);
namespace cheat {
class Seckill : public Module {
public:
  Seckill();
};
} // namespace cheat