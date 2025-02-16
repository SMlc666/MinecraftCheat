#pragma once
#include "game/minecraft/actor/mob/mob.hpp"
#include "game/minecraft/actor/player/LayeredAbilities.hpp"
#include "game/minecraft/actor/player/PlayerInventory.hpp"
#include "game/minecraft/world/level/GameType.hpp"

#include <string>
class GameMode;
class ItemStack;
class Player : public Mob {
public:
  std::byte padding11CE[0x29A2];

public:
  [[nodiscard]] GameMode &getGameMode() const;
  [[nodiscard]] std::string &getName() const;
  [[nodiscard]] ItemStack *getSelectedItem() const;
  glm::vec3 getEyesPos() const;
  bool canOpenContainerScreen() const;
  LayeredAbilities *getAbilities() const;
  void setPlayerGameType(GameType gameType); //0x10B
  PlayerInventory *getSupplies() const;
};
static_assert(sizeof(Player) == 0x3B70);