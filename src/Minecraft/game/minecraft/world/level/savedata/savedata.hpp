#pragma once
#include <string>
#include "base/alias.hpp"
#include "game/common/nbt/CompoundTag.hpp"
class SavedData {
public:
  alias::TypedStorage<1, 1, bool> mDirty;
  alias::TypedStorage<8, 32, ::std::string> mId;

public:
  virtual ~SavedData();
  virtual void deserialize(CompoundTag const &) = 0;
  virtual void serialize(CompoundTag &) const = 0;
};