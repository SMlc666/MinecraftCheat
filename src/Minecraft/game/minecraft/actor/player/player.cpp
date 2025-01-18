#include "player.hpp"
#include "signature.hpp"
GameMode &Player::getGameMode() const {
  using function = GameMode &(*)(const Player *);
  auto func = getSign<function>("Player::getGameMode");
  return func(this);
}