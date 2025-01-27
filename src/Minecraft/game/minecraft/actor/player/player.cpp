#include "player.hpp"
#include "signature.hpp"
GameMode &Player::getGameMode() const {
  using function = GameMode &(*)(const Player *);
  auto func = getSign<function>("Player::getGameMode");
  return func(this);
}
std::string &Player::getName() const {
  using function = std::string &(*)(const Player *);
  auto func = getSign<function>("Player::getName");
  return func(this);
}
ItemStack *Player::getSelectedItem() const {
  using function = ItemStack *(*)(const Player *);
  auto func = reinterpret_cast<function>(vtable[97]);
  return func(this);
}