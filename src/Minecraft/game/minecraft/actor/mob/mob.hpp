#pragma once
#include "game/minecraft/actor/actor.hpp"
class CompassSpriteCalculator;
class ClockSpriteCalculator;
class BuiltInMobComponents;
class MobSpawnMethod;
class BlockPos;
class Mob : public Actor {
public:
  alias::TypedStorage<4, 4, int> mHurtDuration;
  alias::TypedStorage<4, 4, float> mHurtDir;
  alias::TypedStorage<4, 4, float> mOTilt;
  alias::TypedStorage<4, 4, float> mTilt;
  alias::TypedStorage<8, 32, CompassSpriteCalculator> mCompassSpriteCalc;
  alias::TypedStorage<8, 32, CompassSpriteCalculator> mRecoveryCompassSpriteCalc;
  alias::TypedStorage<4, 12, ClockSpriteCalculator> mClockSpriteCalc;
  alias::TypedStorage<4, 4, float> mAttackAnim;
  alias::TypedStorage<4, 4, int> mSwingTime;
  alias::TypedStorage<4, 4, int> mDeathTime;
  alias::TypedStorage<8, 16, BuiltInMobComponents> mBuiltInMobComponents;
  alias::TypedStorage<4, 4, float> mMovementComponentCurrentSpeed;
  alias::TypedStorage<1, 1, bool> mSwinging;
  alias::TypedStorage<1, 1, bool> mSurfaceMob;
  alias::TypedStorage<1, 1, bool> mNaturallySpawned;
  alias::TypedStorage<1, 1, bool> mWantsToBeJockey;
  alias::TypedStorage<1, 1, bool> mSpawnedXP;
  alias::TypedStorage<1, 1, bool> mHasBoundOrigin;
  alias::TypedStorage<1, 2, std::optional<bool>> mActuallyDoKnockbackOrNotReallyBadHackDoNotUse;
  alias::TypedStorage<1, 1, MobSpawnMethod> mSpawnMethod;
  alias::TypedStorage<1, 1, bool> mCreateAiOnReload;
  alias::TypedStorage<8, 8, ActorUniqueID> mCaravanHead;
  alias::TypedStorage<8, 8, ActorUniqueID> mCaravanTail;
  alias::TypedStorage<4, 4, float> mOAttackAnim;
  alias::TypedStorage<4, 12, BlockPos> mBoundOrigin;
  alias::TypedStorage<8, 8, ActorUniqueID> mTargetCaptainId;
};