#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include <cstdint>
class Minecraft;
class ClientInstance {
public:
  uintptr_t **vtable;
  std::byte padding0[168];
  Minecraft *minecraftPtr;

public:
  [[nodiscard]] LocalPlayer *getLocalPlayer() const;
  [[nodiscard]] BlockSource *getRegion() const;
};

static_assert(offsetof(ClientInstance, minecraftPtr) == 0xB0, "Minecraft pointer offset is wrong");