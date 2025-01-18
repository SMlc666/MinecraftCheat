#pragma once
#include <string>
#include "LimboEntitiesVersion.hpp"
#include "game/minecraft/world/level/storage/StorageVersion.hpp"
#include "game/minecraft/world/level/BlockChangedEventTarget.hpp"
class ILevel;
class DimensionType;
class DimensionHeightRange;
class Scheduler;
namespace mce {
class Color;
}

class Dimension {
public:
public:
  Dimension *Dimension_(ILevel &level, DimensionType dimId, DimensionHeightRange heightRange,
                        Scheduler &callbackContext, std::string dimensionName);

public:
  virtual ~Dimension();
  virtual bool isNaturalDimension() const;
  virtual DimensionType getDimensionId() const;
  virtual void sendPacketForPosition(class BlockPos const &, class Packet const &,
                                     class Player const *);
  virtual void flushLevelChunkGarbageCollector();
  virtual void initializeWithLevelStorageManager(class LevelStorageManager &);
  virtual void init();
  virtual void tick();
  virtual void tickRedstone();
  virtual std::unique_ptr<class WorldGenerator> createGenerator() = 0;
  virtual void upgradeLevelChunk(class ChunkSource &, class LevelChunk &, class LevelChunk &) = 0;
  virtual void fixWallChunk(class ChunkSource &, class LevelChunk &) = 0;
  virtual bool levelChunkNeedsUpgrade(class LevelChunk const &) const = 0;
  virtual bool isValidSpawn(int, int) const;
  virtual class mce::Color getBrightnessDependentFogColor(class mce::Color const &, float) const;
  virtual bool isFoggyAt(int, int) const;
  virtual bool hasPrecipitationFog() const;
  virtual short getCloudHeight() const;
  virtual class HashedString getDefaultBiome() const;
  virtual bool mayRespawnViaBed() const;
  virtual bool hasGround() const;
  virtual class BlockPos getSpawnPos() const;
  virtual int getSpawnYPosition() const;
  virtual bool hasBedrockFog();
  virtual float getClearColorScale();
  virtual bool showSky() const;
  virtual bool isDay() const;
  virtual float getTimeOfDay(int, float) const;
  virtual float getSunIntensity(float, class Vec3 const &, float) const;
  virtual bool forceCheckAllNeighChunkSavedStat() const;
  virtual class Vec3 translatePosAcrossDimension(class Vec3 const &, DimensionType) const = 0;
  virtual void sendBroadcast(class Packet const &, class Player *);
  virtual bool is2DPositionRelevantForPlayer(class BlockPos const &, class Player &) const;
  virtual bool isActorRelevantForPlayer(class Player &, class Actor const &) const;
  virtual class BaseLightTextureImageBuilder *getLightTextureImageBuilder() const;
  virtual class DimensionBrightnessRamp const &getBrightnessRamp() const;
  virtual void startLeaveGame();
  virtual std::unique_ptr<class ChunkBuildOrderPolicyBase> _createChunkBuildOrderPolicy();
  virtual void _upgradeOldLimboEntity(class CompoundTag &, LimboEntitiesVersion) = 0;
  virtual std::unique_ptr<class ChunkSource>
      _wrapStorageForVersionCompatibility(std::unique_ptr<class ChunkSource>, StorageVersion) = 0;
  virtual void onBlockChanged(class BlockSource &source, class BlockPos const &pos, uint layer,
                              class Block const &block, class Block const &oldBlock,
                              int updateFlags, struct ActorBlockSyncMessage const *syncMsg,
                              BlockChangedEventTarget eventTarget, class Actor *blockChangeSource);
  virtual void onBlockEvent(class BlockSource &source, int x, int y, int z, int b0, int b1);
  virtual void onChunkLoaded(class ChunkSource &source, class LevelChunk &lc);
  virtual void onLevelDestruction(std::string const &);
  virtual void deserialize(class CompoundTag const &);
  virtual void serialize(class CompoundTag &tag) const;
};