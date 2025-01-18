#pragma once
#include "game/minecraft/actor/actor.hpp"
#include "game/minecraft/actor/mob/mob.hpp"
#include "base/core/platform.hpp"
#include "game/common/gamerefs/OwnerPtr.hpp"
class ChunkViewSource;
class ContainerManagerModel;
class ContainerManagerSubscriber;
class PlayerInventory;
class InventoryOptions;
class ItemInstance;
class ItemGroup;
class Stopwatch;
class DimensionType;
class ActorType;
class PlayerSpawnFallbackType;
class PlayerItemInUse;
class PlayerDestroyProgressCacheComponent;
class ItemStackNetManagerBase;
class NetworkChunkPublisher;
class PlayerUIContainer;
class GameMode;
class HudContainerManagerModel;
class EnderChestContainer;
class InventoryTransactionManager;
class PlayerRespawnRandomizer;
class PlayerListener;
class SubClientId;
class PacketSender;
class SerializedSkin;
class BlockSource;
class Player : public Mob {
public:
  struct PlayerSpawnPoint {
  public:
    alias::TypedStorage<4, 12, ::BlockPos> mSpawnBlockPos;
    alias::TypedStorage<4, 12, ::BlockPos> mPlayerPosition;
    alias::TypedStorage<4, 4, ::DimensionType> mDimension;
  };
  struct CachedSpawnData;
  enum class SpawnPositionState : int {
    InitializeSpawnPositionRandomizer = 0,
    WaitForClientAck = 1,
    DetermineDimension = 2,
    ChangeDimension = 3,
    WaitForDimension = 4,
    ChooseSpawnArea = 5,
    CheckLoadedChunk = 6,
    ChooseSpawnPosition = 7,
    SpawnComplete = 8,
  };
  enum class SpawnPositionSource : int {
    Randomizer = 0,
    Respawn = 1,
    Teleport = 2,
    Static = 3,
  };

public:
  alias::TypedStorage<8, 24, std::vector<ushort>> mOceanBiomes;
  alias::TypedStorage<8, 24, std::vector<ushort>> mFroglights;
  alias::TypedStorage<4, 4, float const> mSneakHeight;
  alias::TypedStorage<4, 4, float const> mSneakOffset;
  alias::TypedStorage<4, 4, int> mScore;
  alias::TypedStorage<4, 4, float> mOBob;
  alias::TypedStorage<4, 4, float> mBob;
  alias::TypedStorage<4, 4, BuildPlatform> mBuildPlatform;
  alias::TypedStorage<8, 32, std::string> mUniqueName;
  alias::TypedStorage<8, 32, std::string> mServerId;
  alias::TypedStorage<8, 32, std::string> mSelfSignedId;
  alias::TypedStorage<8, 32, std::string> mPlatformOfflineId;
  alias::TypedStorage<8, 8, uint64> mClientRandomId;
  alias::TypedStorage<8, 32, std::string> mPlatformId;
  alias::TypedStorage<8, 8, ActorUniqueID> mPendingVehicleID;
  alias::TypedStorage<8, 8, ActorUniqueID> mPendingLeftShoulderPassengerID;
  alias::TypedStorage<8, 8, ActorUniqueID> mPendingRightShoulderPassengerID;
  alias::TypedStorage<8, 8, ActorUniqueID> mInteractTarget;
  alias::TypedStorage<4, 12, Vec3> mInteractTargetPos;
  alias::TypedStorage<1, 1, bool> mHasFakeInventory;
  alias::TypedStorage<1, 1, bool> mIsRegionSuspended;
  alias::TypedStorage<1, 1, bool> mUpdateMobs;
  alias::TypedStorage<8, 16, std::shared_ptr<ChunkViewSource>> mChunkSource;
  alias::TypedStorage<8, 16, std::shared_ptr<ChunkViewSource>> mSpawnChunkSource;
  alias::TypedStorage<4, 12, Vec3> mCapePosO;
  alias::TypedStorage<4, 12, Vec3> mCapePos;
  alias::TypedStorage<8, 16, std::shared_ptr<ContainerManagerModel>> mContainerManager;
  alias::TypedStorage<8, 8, std::unique_ptr<ContainerManagerSubscriber>>
      mContainerManagerSubscribers;
  alias::TypedStorage<8, 8, std::unique_ptr<PlayerInventory>> mInventory;
  alias::TypedStorage<4, 20, InventoryOptions> mInventoryOptions;
  alias::TypedStorage<4, 4, float> mDistanceSinceTransformEvent;
  alias::TypedStorage<8, 24, std::vector<ItemInstance>> mCreativeItemList;
  alias::TypedStorage<8, 96, std::array<std::vector<ItemGroup>, 4>> mFilteredCreativeItemList;
  alias::TypedStorage<8, 32, std::string> mPlatformOnlineId;
  alias::TypedStorage<4, 4, Player::SpawnPositionState> mSpawnPositionState;
  alias::TypedStorage<4, 4, Player::SpawnPositionSource> mSpawnPositionSource;
  alias::TypedStorage<4, 12, Vec3> mSpawnPositioningTestPosition;
  alias::TypedStorage<4, 4, uint> mRespawnChunkBuilderPolicyHandle;
  alias::TypedStorage<4, 64, Player::CachedSpawnData> mCachedSpawnData;
  alias::TypedStorage<8, 16, OwnerPtr<BlockSource>> mSpawnBlockSource;
  alias::TypedStorage<8, 56, Stopwatch> mRespawnStopwatch_Searching;
  alias::TypedStorage<4, 12, Vec3> mRespawnOriginalPosition;
  alias::TypedStorage<4, 4, DimensionType> mRespawnOriginalDimension;
  alias::TypedStorage<8, 32, std::string> mRespawnMessage;
  alias::TypedStorage<1, 1, bool> mRespawnReady;
  alias::TypedStorage<1, 1, bool> mIsInitialSpawnDone;
  alias::TypedStorage<1, 1, bool> mRespawningFromTheEnd;
  alias::TypedStorage<1, 1, bool> mPositionLoadedFromSave;
  alias::TypedStorage<1, 1, bool> mBlockRespawnUntilClientMessage;
  alias::TypedStorage<1, 1, bool> mHasSeenCredits;
  alias::TypedStorage<4, 8, std::optional<PlayerSpawnFallbackType>> mSpawnFallbackType;
  alias::TypedStorage<4, 16, std::optional<Vec3>> mSpawnFallbackPosition;
  alias::TypedStorage<8, 168, PlayerItemInUse> mItemInUse;
  alias::TypedStorage<4, 4, ActorType> mLastHurtBy;
  alias::TypedStorage<2, 2, short> mSleepCounter;
  alias::TypedStorage<2, 2, short> mPrevSleepCounter;
  alias::TypedStorage<8, 8, ActorUniqueID> mPreviousInteractEntity;
  alias::TypedStorage<4, 4, int> mPreviousCarriedItem;
  alias::TypedStorage<4, 4, int> mEmoteTicks;
  alias::TypedStorage<8, 16, std::shared_ptr<NetworkChunkPublisher>> mChunkPublisherView;
  alias::TypedStorage<8, 8, PacketSender &> mPacketSender;
  alias::TypedStorage<8, 16, std::shared_ptr<HudContainerManagerModel>> mHudContainerManagerModel;
  alias::TypedStorage<8, 8, std::unique_ptr<EnderChestContainer>> mEnderChestInventory;
  alias::TypedStorage<8, 24, std::vector<ActorUniqueID>> mTrackedBossIDs;
  alias::TypedStorage<8, 136, ItemGroup> mCursorSelectedItemGroup;
  alias::TypedStorage<8, 480, PlayerUIContainer> mPlayerUIContainer;
  alias::TypedStorage<8, 48, InventoryTransactionManager> mTransactionManager;
  alias::TypedStorage<8, 8, std::unique_ptr<GameMode>> mGameMode;
  alias::TypedStorage<8, 8, std::unique_ptr<PlayerRespawnRandomizer>> mSpawnRandomizer;
  alias::TypedStorage<8, 8, std::unique_ptr<SerializedSkin>> mSkin;
  alias::TypedStorage<8, 8, std::unique_ptr<ItemStackNetManagerBase>> mItemStackNetManager;
  alias::TypedStorage<8, 16, std::shared_ptr<AnimationComponent>> mUIAnimationComponent;
  alias::TypedStorage<8, 16, std::shared_ptr<AnimationComponent>> mMapAnimationComponent;
  alias::TypedStorage<4, 28, Player::PlayerSpawnPoint> mPlayerRespawnPoint;
  alias::TypedStorage<4, 4, float> mServerBuildRatio;
  alias::TypedStorage<1, 1, SubClientId> mClientId;
  alias::TypedStorage<1, 1, bool> mInteractDataDirty;
  alias::TypedStorage<1, 1, bool> mShouldClientGenerateChunks;
  alias::TypedStorage<1, 1, bool> mUseMapAnimationComponent;
  alias::TypedStorage<1, 1, bool> mIsDeferredRenderingFirstPersonObjects;
  alias::TypedStorage<1, 1, bool> mDestroyingBlock;
  alias::TypedStorage<1, 1, bool> mPlayerLevelChanged;
  alias::TypedStorage<4, 4, int> mPreviousLevelRequirement;
  alias::TypedStorage<4, 4, int> mLastLevelUpTime;
  alias::TypedStorage<8, 16, std::shared_ptr<AnimationComponent>> mFirstPersonAnimationComponent;
  alias::TypedStorage<8, 24, std::vector<PlayerListener *>> mListeners;
  alias::TypedStorage<4, 12, Vec3> mRespawnPositionCandidate;
  alias::TypedStorage<4, 12, Vec3> mEnterBedPosition;
  alias::TypedStorage<4, 12, Vec3> mPreDimensionTransferSpawnPosition;
  alias::TypedStorage<4, 4, int> mEnchantmentSeed;
  alias::TypedStorage<8, 24, std::vector<uint>> mOnScreenAnimationTextures;
  alias::TypedStorage<4, 4, int> mOnScreenAnimationTicks;
  alias::TypedStorage<4, 4, uint> mChunkRadius;
  alias::TypedStorage<4, 4, int> mMapIndex;
  alias::TypedStorage<4, 4, float> mElytraVolume;
  alias::TypedStorage<8, 8, uint64> mElytraLoop;
  alias::TypedStorage<8, 64, std::unordered_map<HashedString, int>> mCooldowns;
  alias::TypedStorage<8, 64, std::unordered_map<HashedString, HashedString>> mVanillaCooldowns;
  alias::TypedStorage<8, 8, int64> mStartedBlockingTimeStamp;
  alias::TypedStorage<8, 8, int64> mBlockedUsingShieldTimeStamp;
  alias::TypedStorage<8, 8, int64> mBlockedUsingDamagedShieldTimeStamp;
  alias::TypedStorage<8, 32, std::string> mName;
  alias::TypedStorage<8, 32, std::string> mLastEmotePlayed;
  alias::TypedStorage<8, 8, int64> mEmoteEasterEggEndTime;
  alias::TypedStorage<4, 4, uint> mEmoteMessageCount;
  alias::TypedStorage<8, 32, std::string> mDeviceId;
  alias::TypedStorage<1, 1, bool> mFlagClientForBAIReset;
  alias::TypedStorage<1, 1, bool> mSendInventoryOptionsToClient;
  alias::TypedStorage<1, 1, bool> mIsHostingPlayer;
  alias::TypedStorage<1, 1, bool> mPrevBlockedUsingShield;
  alias::TypedStorage<1, 1, bool> mPrevBlockedUsingDamagedShield;
  alias::TypedStorage<1, 1, bool> mUsedPotion;
  alias::TypedStorage<8, 8, PlayerDestroyProgressCacheComponent &> mDestroyProgressCache;
};