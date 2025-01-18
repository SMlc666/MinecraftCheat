#include "player.hpp"
#include "signature.hpp"
GameMode &Player::getGameMode() const {
  using function = GameMode &(*)(const Player *);
  auto func = getSign<function>("Player::getGameMode");
  return func(this);
}
std::string Player::getName() const {
  using function = std::string (*)(const Player *);
  auto func = getSign<function>("Player::getName");
  return func(this);
}