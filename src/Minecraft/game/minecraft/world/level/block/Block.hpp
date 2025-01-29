#pragma once
#include "gsl/pointers"
#include <cstddef>
class BlockLegacy;
class Block {
public:
  uintptr_t **vtable;
  std::byte padding0[0x20];
  gsl::not_null<BlockLegacy *> mBlockLegacy; // ida Block::Block a3
};
static_assert(offsetof(Block, mBlockLegacy) == 0x28);