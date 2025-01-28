#pragma once

#include "game/minecraft/world/level/block/BlockPos.hpp"
#include <functional>
class Packet;
class Player;
class Vec3;
class DimensionType;
class ActorUniqueID;
class BiomeRegistry;
class Actor;
class IDimension {
public:
  virtual ~IDimension() = default;
  virtual bool isNaturalDimension() const = 0;
  virtual DimensionType getDimensionId() const = 0;
  virtual void sendPacketForPosition(const BlockPos &, const Packet &, const Player *) = 0;
  virtual void flushLevelChunkGarbageCollector() = 0;
  virtual void initializeWithLevelStorageManager(class LevelStorageManager &) = 0;
  virtual BiomeRegistry &getBiomeRegistry() = 0;
  virtual const BiomeRegistry &getBiomeRegistry() const = 0;
  virtual Vec3 translatePosAcrossDimension(const Vec3 &, DimensionType) const = 0;
  virtual Actor *fetchEntity(ActorUniqueID actorID, bool getRemoved) const = 0;
};