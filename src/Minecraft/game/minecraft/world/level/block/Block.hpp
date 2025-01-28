#pragma once
#include <cstddef>
class BlockLegacy;
class Block {
public:
  std::byte padding0[0x20];
  BlockLegacy *mBlockLegacy;
};
static_assert(offsetof(Block, mBlockLegacy) == 0x20);