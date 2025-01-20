#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include <cstdint>
class Minecraft;
class ClientInstance {
public:
  uintptr_t **vtable;
  std::byte padding0[176];
  Minecraft *minecraftPtr; // 将偏移量44处的4字节改为Minecraft*指针
public:
  [[nodiscard]] LocalPlayer *getLocalPlayer() const;
};

static_assert(offsetof(ClientInstance, minecraftPtr) == 0xB0 + 8,
              "Minecraft pointer offset is wrong");