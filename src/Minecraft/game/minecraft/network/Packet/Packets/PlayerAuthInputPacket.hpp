#pragma once
#include "base/alias.hpp"
#include "game/deps/input/InputMode.hpp"
#include "game/minecraft/input/NewInteractionModel.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "glm/fwd.hpp"
#include "game/minecraft/client/game/ClientPlayMode.hpp"
#include <bitset>
class ItemStackRequestData;
class PackedItemUseLegacyInventoryTransaction;
class PlayerAuthInputPacket : public Packet {
public:
  enum class InputData : int {
    Ascend = 0,
    Descend = 1,
    NorthJumpDeprecated = 2,
    JumpDown = 3,
    SprintDown = 4,
    ChangeHeight = 5,
    Jumping = 6,
    AutoJumpingInWater = 7,
    Sneaking = 8,
    SneakDown = 9,
    Up = 10,
    Down = 11,
    Left = 12,
    Right = 13,
    UpLeft = 14,
    UpRight = 15,
    WantUp = 16,
    WantDown = 17,
    WantDownSlow = 18,
    WantUpSlow = 19,
    Sprinting = 20,
    AscendBlock = 21,
    DescendBlock = 22,
    SneakToggleDown = 23,
    PersistSneak = 24,
    StartSprinting = 25,
    StopSprinting = 26,
    StartSneaking = 27,
    StopSneaking = 28,
    StartSwimming = 29,
    StopSwimming = 30,
    StartJumping = 31,
    StartGliding = 32,
    StopGliding = 33,
    PerformItemInteraction = 34,
    PerformBlockActions = 35,
    PerformItemStackRequest = 36,
    HandledTeleport = 37,
    Emoting = 38,
    MissedSwing = 39,
    StartCrawling = 40,
    StopCrawling = 41,
    StartFlying = 42,
    StopFlying = 43,
    ClientAckServerData = 44,
    IsInClientPredictedVehicle = 45,
    PaddlingLeft = 46,
    PaddlingRight = 47,
    BlockBreakingDelayEnabled = 48,
    HorizontalCollision = 49,
    VerticalCollision = 50,
    DownLeft = 51,
    DownRight = 52,
    StartUsingItem = 53,
    IsCameraRelativeMovementEnabled = 54,
    IsRotControlledByMoveDirection = 55,
    StartSpinAttack = 56,
    StopSpinAttack = 57,
    IsHotbarOnlyTouch = 58,
    JumpReleasedRaw = 59,
    JumpPressedRaw = 60,
    JumpCurrentRaw = 61,
    SneakReleasedRaw = 62,
    SneakPressedRaw = 63,
    SneakCurrentRaw = 64,
    InputNum = 65,
  };

public:
  alias::TypedStorage<4, 8, glm::vec2> mRot;
  alias::TypedStorage<4, 12, glm::vec3> mPos;
  alias::TypedStorage<4, 4, float> mYHeadRot;
  alias::TypedStorage<4, 12, glm::vec3> mPosDelta;
  alias::TypedStorage<4, 8, glm::vec2> mVehicleRot;
  alias::TypedStorage<4, 8, glm::vec2> mAnalogMoveVector;
  alias::TypedStorage<4, 8, glm::vec2> mMove;
  alias::TypedStorage<4, 8, glm::vec2> mInteractRotation;
  alias::TypedStorage<4, 12, glm::vec3> mCameraOrientation;
  alias::TypedStorage<4, 8, glm::vec2> mRawMoveVector;
  alias::TypedStorage<8, 16, std::bitset<65>> mInputData;
  alias::TypedStorage<4, 4, InputMode> mInputMode;
  alias::TypedStorage<4, 4, ClientPlayMode> mPlayMode;
  alias::TypedStorage<4, 4, NewInteractionModel> mNewInteractionModel;
  alias::TypedStorage<8, 8, PlayerInputTick> mClientTick;
  alias::TypedStorage<8, 8, std::unique_ptr<PackedItemUseLegacyInventoryTransaction>>
      mItemUseTransaction;
  alias::TypedStorage<8, 8, std::unique_ptr<ItemStackRequestData>> mItemStackRequest;
  alias::UntypedStorage<8, 24> mPlayerBlockActions;
  alias::UntypedStorage<8, 8> mClientPredictedVehicle;
};