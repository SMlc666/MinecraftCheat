#pragma once
#include "game/minecraft/actor/mob/mob.hpp"

#include <string>
class GameMode;
class ItemStack;
class Player : public Mob {
public:
  [[nodiscard]] GameMode &getGameMode() const;
  [[nodiscard]] std::string &getName() const;
  [[nodiscard]] ItemStack *getSelectedItem() const;
  glm::vec3 getEyesPos() const;
};