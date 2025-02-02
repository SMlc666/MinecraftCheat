#pragma once
#include <array>
#include <cstddef>

class MoveInputHandler {
public:
  std::array<std::byte, 0x28> padding0;
  bool mSneakButtonDown;
  std::array<std::byte, 0x5> padding28;
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

public:
  [[nodiscard]] inline bool isMove() const {
    return forwardLeftInput() || forwardRightInput() || forwardInput() || backwardInput() ||
           leftInput() || rightInput();
  }

  [[nodiscard]] inline bool sneakInput() const {
    return mSneakButtonDown;
  }

  [[nodiscard]] inline bool spaceInput() const {
    return mSpaceButtonDown;
  }

  [[nodiscard]] inline bool forwardLeftInput() const {
    return (mForwardLeftButtonDown || (forwardInput() && leftInput()));
  }

  [[nodiscard]] inline bool forwardRightInput() const {
    return (mForwardRightButtonDown || (forwardInput() && rightInput()));
  }

  [[nodiscard]] inline bool forwardInput() const {
    return mForwardButtonDown;
  }

  [[nodiscard]] inline bool backwardInput() const {
    return mBackwardButtonDown;
  }

  [[nodiscard]] inline bool leftInput() const {
    return (mLeftButtonDown && !forwardInput() && !backwardInput());
  }

  [[nodiscard]] inline bool rightInput() const {
    return (mRightButtonDown && !forwardInput() && !backwardInput());
  }

  [[nodiscard]] inline bool upInput() const {
    return mUpButtonDown;
  }

  [[nodiscard]] inline bool downInput() const {
    return mDownButtonDown;
  }

  [[nodiscard]] inline bool cInput() const {
    return mCButtonDown;
  }
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