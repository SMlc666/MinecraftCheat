#pragma once
#include "game/common/SubClientId.hpp"
#include "game/deps/core/Bedrock/Result.hpp"
#include "game/minecraft/network/Compressibility.hpp"
#include "game/minecraft/network/Packet/MinecraftPacketIds.hpp"
#include "game/deps/raknet/PacketPriority.hpp"
#include "game/minecraft/network/NetworkPeer.hpp"
#include <string>
class ReadOnlyBinaryStream;
class BinaryStream;
class IPacketHandlerDispacher;
#pragma pack(push, 4)
class Packet {
public:
  std::byte padding8[0x24];

public:
  virtual ~Packet();
  virtual MinecraftPacketIds getId() const = 0;
  virtual std::string getName() const = 0;
  virtual void write(BinaryStream &stream) const = 0;
  virtual Bedrock::Result<void, std::error_code> read(ReadOnlyBinaryStream &bitStream);
  virtual bool disallowBatching() const;
  virtual bool isValid() const;
  virtual Bedrock::Result<void, std::error_code> _read(ReadOnlyBinaryStream &stream) = 0;
};
#pragma pack(pop)
static_assert(sizeof(Packet) == 0x2C);