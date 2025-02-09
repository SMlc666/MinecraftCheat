#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include <cstdint>
class Minecraft;
#pragma pack(push, 1)
class ClientInstance {
public:
  uintptr_t **vtable;
  std::byte padding0[168];
  Minecraft *minecraftPtr;
  std::byte paddingB8[0x700];
  glm::mat4 *mMatrix;
  std::byte padding7C0[0x38];
  float *mHfov;
  std::byte padding800[0xC];
  float *mVfov;

public:
  [[nodiscard]] LocalPlayer *getLocalPlayer() const;
  [[nodiscard]] BlockSource *getRegion() const;
};
#pragma pack(pop)
static_assert(offsetof(ClientInstance, minecraftPtr) == 0xB0, "Minecraft pointer offset is wrong");
static_assert(offsetof(ClientInstance, mMatrix) == 0x7B8, "Matrix pointer offset is wrong");
static_assert(offsetof(ClientInstance, mHfov) == 0x7F8, "HFOV pointer offset is wrong");
static_assert(offsetof(ClientInstance, mVfov) == 0x80C, "VFOV pointer offset is wrong");