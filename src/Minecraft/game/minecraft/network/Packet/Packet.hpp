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
class Packet {
public:
  PacketPriority mPriority;
  NetworkPeer::Reliability mReliability;
  SubClientId mClientSubId;
  bool mIsHandled;
  std::chrono::steady_clock::time_point mReceiveTimepoint;
  IPacketHandlerDispacher *mHandler;
  Compressibility mCompressible;

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