#pragma once

#include "game/common/ActorRuntimeID.hpp"
#include "game/deps/core/Bedrock/Result.hpp"
#include "game/minecraft/actor/actor.hpp"
#include "game/minecraft/network/Packet/MinecraftPacketIds.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include <system_error>
class AnimatePacket : public Packet {
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
  std::byte padding2C[0x1C];

public:
  virtual ~AnimatePacket();
  virtual MinecraftPacketIds getId() const;
  virtual std::string getName() const;
  virtual void write(BinaryStream &stream) const;
  virtual Bedrock::Result<void, std::error_code> _read(ReadOnlyBinaryStream &stream);

public:
  AnimatePacket(Action action, Actor &e);
};
static_assert(sizeof(AnimatePacket) == 0x48);