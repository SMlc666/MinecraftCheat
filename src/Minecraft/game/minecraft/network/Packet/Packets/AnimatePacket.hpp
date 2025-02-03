#pragma once

#include "game/common/ActorRuntimeID.hpp"
#include "game/deps/core/Bedrock/Result.hpp"
#include "game/minecraft/actor/actor.hpp"
#include "game/minecraft/network/Packet/MinecraftPacketIds.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "signature.hpp"
#include <system_error>
class AnimatePacket {
public:
  enum class Action : int {
    NoAction = 0,
    Swing = 1,
    WakeUp = 3,
    CriticalHit = 4,
    MagicCriticalHit = 5,
    RowRight = 128,
    RowLeft = 129,
  };

public:
  std::byte padding8[0x2C];
  std::byte padding2C[0x1C];

public:
  inline AnimatePacket(Action action, Actor &e) {
    using function = void (*)(AnimatePacket *, Action, Actor &);
    auto func = getSign<function>("AnimatePacket::AnimatePacket");
    func(this, action, e);
  }
};
static_assert(sizeof(AnimatePacket) == 0x48);