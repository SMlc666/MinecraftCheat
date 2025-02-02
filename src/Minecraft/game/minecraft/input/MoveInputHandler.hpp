#pragma once
#include <cstddef>

class MoveInputHandler {
public:
  std::byte padding0[0x28];
  bool mSneakButtonDown;
  std::byte padding28[0x5];
  bool mSpaceButtonDown;
  std::byte padding2F;
  bool mForwardLeftButtonDown;
  bool mForwardRightButtonDown;
  bool mForwardButtonDown;
  bool mBackwardButtonDown;
  bool mLeftButtonDown;
  bool mRightButtonDown;
  bool mUpButtonDown;
  bool mDownButtonDown;
  bool mCButtonDown;
};

static_assert(offsetof(MoveInputHandler, mSneakButtonDown) == 40);
static_assert(offsetof(MoveInputHandler, mSpaceButtonDown) == 46);
static_assert(offsetof(MoveInputHandler, mForwardLeftButtonDown) == 48);
static_assert(offsetof(MoveInputHandler, mForwardRightButtonDown) == 49);
static_assert(offsetof(MoveInputHandler, mForwardButtonDown) == 50);
static_assert(offsetof(MoveInputHandler, mBackwardButtonDown) == 51);
static_assert(offsetof(MoveInputHandler, mLeftButtonDown) == 52);
static_assert(offsetof(MoveInputHandler, mRightButtonDown) == 53);
static_assert(offsetof(MoveInputHandler, mUpButtonDown) == 54);
static_assert(offsetof(MoveInputHandler, mDownButtonDown) == 55);
static_assert(offsetof(MoveInputHandler, mCButtonDown) == 56);