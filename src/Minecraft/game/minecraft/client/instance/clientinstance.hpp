#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include <cstdint>
class ClientInstance {
public:
  uintptr_t **vtable;

public:
  [[nodiscard]] LocalPlayer *getLocalPlayer() const;
};