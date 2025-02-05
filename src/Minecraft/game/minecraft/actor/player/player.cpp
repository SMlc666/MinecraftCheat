#include "player.hpp"
#include "glm/fwd.hpp"
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
glm::vec3 Player::getEyesPos() const {
  glm::vec3 pos = getPosition();
  return {pos.x, pos.y + 1.8F, pos.z};
}