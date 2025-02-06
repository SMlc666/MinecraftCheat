#pragma once
#include "game/common/ActorRuntimeID.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "game/minecraft/world/level/Tick.h"
#include "glm/glm.hpp"
#include "game/minecraft/world/entity/PlayerPositionModeComponent.hpp"
class MovePlayerPacket : public Packet {
public:
  ActorRuntimeID mPlayerID;                                 // this+0x30
  glm::vec3 mPos;                                           // this+0x38
  glm::vec2 mRot;                                           // this+0x44
  float mYHeadRot;                                          // this+0x4C
  PlayerPositionModeComponent::PositionMode mResetPosition; // this+0x50
  bool mOnGround;                                           // this+0x51
  ActorRuntimeID mRidingID;                                 // this+0x58
  int mCause;                                               // this+0x60
  int mSourceEntityType;                                    // this+0x64
  Tick mTick;                                               // this+0x68
};
static_assert(sizeof(MovePlayerPacket) == 0x70);