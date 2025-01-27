#pragma once
#include "game/minecraft/actor/actor.hpp"
class BlockPos;
class Block;
class BlockLegacy;
class BlockSource {
public:
  std::byte padding8[0x18];
  BlockLegacy *mBlockLegacy;

public:
  virtual ~BlockSource();
  virtual Block *getBlock(int x, int y, int z);
  virtual Block *getBlock(const BlockPos &pos);
  virtual Block *getBlock(const BlockPos &pos, int);
};