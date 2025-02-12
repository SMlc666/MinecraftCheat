#include "player.hpp"
#include "game/minecraft/actor/player/LayeredAbilities.hpp"
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
bool Player::canOpenContainerScreen() const {
  using function = bool (*)(const Player *);
  auto func = getSign<function>("Player::canOpenContainerScreen");
  return func(this);
}
LayeredAbilities *Player::getAbilities() const {
  using function = LayeredAbilities *(*)(const Player *);
  auto func = getSign<function>("Player::getAbilities");
  return func(this);
}
void Player::setPlayerGameType(GameType gameType) {
  using function = void (*)(Player *, GameType);
  auto func = reinterpret_cast<function>(vtable[267]);
  func(this, gameType);
}