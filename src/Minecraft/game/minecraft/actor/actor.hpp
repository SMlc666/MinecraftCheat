#pragma once
#include "actorflags.hpp"
#include "base/alias.hpp"
#include "base/mcint.hpp"
#include "glm/fwd.hpp"
#include "game/common/gamerefs/WeakRef.hpp"
#include "SpawnRuleEnum.hpp"

#include <map>
class EntityContext;
class VariantParameterList;
class ActorInitializationMethod;
class ActorDefinitionGroup;
class ActorDefinitionDescriptor;
class SynchedActorDataEntityWrapper;
class RopeSystem;
class Vec3;
class Dimension;
class ActorUniqueID;
class ILevel;
class ActorCategory;
class AnimationComponent;
class ActorDefinitionDiffList;
class ActionQueue;
namespace mce {
class Color;
}
class BuiltInActorComponents;
class ActorTerrainInterlockData;
class MolangVariableMap;
class ActorDefinitionPtr;
class ActorDamageCause;
class AABB;
class CompoundTag;
class SpatialActorNetworkData;
class HashedString;
class Actor {
public:
  //NOLINTBEGIN
  alias::TypedStorage<8, 24, EntityContext> mEntityContext;
  alias::TypedStorage<8, 72, VariantParameterList> mInitParams;
  alias::TypedStorage<8, 32, std::string> mCustomInitEventName;
  alias::TypedStorage<1, 1, ActorInitializationMethod> mInitMethod;
  alias::TypedStorage<1, 1, bool> mForceInitMethodToSpawnOnReload;
  alias::TypedStorage<1, 1, bool> mAdded;
  alias::TypedStorage<8, 8, ActorDefinitionGroup *> mDefinitions;
  alias::TypedStorage<8, 8, std::unique_ptr<ActorDefinitionDescriptor>> mCurrentDescription;
  alias::TypedStorage<8, 16, std::shared_ptr<RopeSystem>> mLeashRopeSystem;
  alias::TypedStorage<8, 32, std::string> mAlias;
  alias::TypedStorage<4, 68, std::optional<glm::mat4x4>> mPreviousRenderTransform;
  alias::TypedStorage<4, 4, int> mLastHurtByPlayerTime;
  alias::TypedStorage<8, 16, std::map<HashedString, std::vector<std::vector<glm::mat4x4>>>>
      mPreviousBoneMatrices;
  alias::TypedStorage<8, 48, SynchedActorDataEntityWrapper> mEntityData;
  alias::TypedStorage<8, 8, std::unique_ptr<SpatialActorNetworkData>> mNetworkData;
  alias::TypedStorage<4, 12, Vec3> mSentDelta;
  alias::TypedStorage<4, 4, float> mScale;
  alias::TypedStorage<4, 4, float> mScalePrev;
  alias::TypedStorage<1, 1, bool> mIgnoreLighting;
  alias::TypedStorage<1, 1, bool> mFilterLighting;
  alias::TypedStorage<4, 4, float> mStepSoundVolume;
  alias::TypedStorage<4, 4, float> mStepSoundPitch;
  alias::TypedStorage<8, 8, AABB *> mLastHitBB;
  alias::TypedStorage<8, 8, uint64> mNameTagHash;
  alias::TypedStorage<4, 4, float> mShadowOffset;
  alias::TypedStorage<4, 4, float> mPushthrough;
  alias::TypedStorage<4, 4, int> mTickCount;
  alias::TypedStorage<4, 4, int> mInvulnerableTime;
  alias::TypedStorage<4, 4, int> mLastHealth;
  alias::TypedStorage<1, 1, bool> mHurtMarked;
  alias::TypedStorage<1, 1, bool> mWasHurtLastFrame;
  alias::TypedStorage<1, 1, bool> mInvulnerable;
  alias::TypedStorage<4, 4, int> mFlameTexFrameIndex;
  alias::TypedStorage<4, 4, float> mFlameFrameIncrementTime;
  alias::TypedStorage<1, 1, bool> mAlwaysFireImmune;
  alias::TypedStorage<1, 1, bool> mInheritRotationWhenRiding;
  alias::TypedStorage<1, 1, bool> mForcedLoading;
  alias::TypedStorage<1, 1, bool> mForceSendMotionPacket;
  alias::TypedStorage<1, 1, bool> mHighlightedThisFrame;
  alias::TypedStorage<1, 1, bool> mInitialized;
  alias::TypedStorage<1, 1, bool> mProcessedOnChunkDiscard;
  alias::TypedStorage<4, 4, float> mSoundVolume;
  alias::TypedStorage<4, 4, int> mShakeTime;
  alias::TypedStorage<8, 8, ActorUniqueID> mLegacyUniqueID;
  alias::TypedStorage<8, 16, WeakRef<Dimension>> mDimension;
  alias::TypedStorage<8, 8, ILevel *> mLevel;
  alias::TypedStorage<8, 48, HashedString> mActorRendererId;
  alias::TypedStorage<4, 4, ActorCategory> mCategories;
  alias::TypedStorage<8, 32, BuiltInActorComponents> mBuiltInComponents;
  alias::TypedStorage<8, 48, HashedString> mActorRendererIdThatAnimationComponentWasInitializedWith;
  alias::TypedStorage<1, 1, bool> mChanged;
  alias::TypedStorage<1, 1, bool> mRemoved;
  alias::TypedStorage<1, 1, bool> mMovedToLimbo;
  alias::TypedStorage<1, 1, bool> mMovedToUnloadedChunk;
  alias::TypedStorage<1, 1, bool> mBlocksBuilding;
  alias::TypedStorage<8, 16, std::shared_ptr<AnimationComponent>> mAnimationComponent;
  alias::TypedStorage<8, 16, std::shared_ptr<AnimationComponent>> mUIAnimationComponent;
  alias::TypedStorage<8, 8, ActorUniqueID> mTargetId;
  alias::TypedStorage<8, 8, ActorUniqueID> mInLovePartner;
  alias::TypedStorage<8, 8, std::unique_ptr<CompoundTag>> mPersistingTradeOffers;
  alias::TypedStorage<4, 4, int> mPersistingTradeRiches;
  alias::TypedStorage<1, 1, bool> mPersistingTrade;
  alias::TypedStorage<1, 1, bool> mEffectsDirty;
  alias::TypedStorage<1, 1, bool> mLootDropped;
  alias::TypedStorage<1, 1, bool> mLoadedFromNBTThisFrame;
  alias::TypedStorage<4, 16, mce::Color> mHurtColor;
  alias::TypedStorage<8, 8, std::unique_ptr<ActorDefinitionDiffList>> mDefinitionList;
  alias::TypedStorage<8, 8, std::unique_ptr<CompoundTag>> mLoadedActorPropertyTag;
  alias::TypedStorage<8, 16, ActorDefinitionPtr> mActorDefinitionPtr;
  alias::TypedStorage<8, 32, std::string> mFilteredNameTag;
  alias::TypedStorage<8, 24, ActorTerrainInterlockData> mTerrainInterlockData;
  alias::TypedStorage<8, 8, ActorUniqueID> mLastHurtMobId;
  alias::TypedStorage<8, 8, ActorUniqueID> mLastHurtByMobId;
  alias::TypedStorage<8, 8, ActorUniqueID> mLastHurtByPlayerId;
  alias::TypedStorage<8, 8, uint64> mLastHurtTimestamp;
  alias::TypedStorage<4, 4, ActorDamageCause> mLastHurtCause;
  alias::TypedStorage<4, 4, float> mLastHurt;
  alias::TypedStorage<4, 4, int> mLastHurtMobTimestamp;
  alias::TypedStorage<4, 4, int> mLastHurtByMobTime;
  alias::TypedStorage<4, 4, int> mLastHurtByMobTimestamp;
  alias::TypedStorage<1, 1, bool> mIsPredictableProjectile;
  alias::TypedStorage<1, 1, bool> mIsRenderingInUI;
  alias::TypedStorage<1, 1, bool> mUpdateEffects;
  alias::TypedStorage<1, 1, bool> mCanPickupItems;
  alias::TypedStorage<1, 1, bool> mHasSetCanPickupItems;
  alias::TypedStorage<1, 1, bool> mChainedDamageEffects;
  alias::TypedStorage<1, 1, bool> mWasInWallLastTick;
  alias::TypedStorage<4, 4, int> mTicksInWall;
  alias::TypedStorage<4, 4, int> mAffectedByWaterBottleTicksToEffect;
  alias::TypedStorage<4, 4, SpawnRuleEnum> mSpawnRulesEnum;
  alias::TypedStorage<8, 8, std::unique_ptr<ActionQueue>> mActionQueue;
  alias::TypedStorage<8, 56, MolangVariableMap> mMolangVariables;
  alias::TypedStorage<8, 8, ActorUniqueID> mFishingHookID;
  //NOLINTEND
public:
  virtual bool getStatusFlag(ActorFlags);
  virtual void setStatusFlag(ActorFlags, bool);
  virtual bool hasComponent(HashedString const &name) const;
  virtual void outOfWorld();
}; // namespace class Actor