#pragma once
#include "base/mcint.hpp"
#include "game/deps/input/InputMode.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include <functional>
class Vec3;
class ItemStack;
class InteractionResult;
class Block;
class Actor;
class GameMode {
public:
  Player *player;
  std::byte padding8[168];

public:
  //vtable index: 0
  virtual ~GameMode();
  //vtable index: 1
  virtual bool startDestroyBlock(BlockPos const &pos, uchar face, bool &hasDestroyedBlock);
  //vtable index: 2
  virtual bool destroyBlock(BlockPos const &pos, uchar face);
  //vtable index: 3
  virtual bool continueDestroyBlock(BlockPos const &pos, uchar face, Vec3 const &playerPos,
                                    bool &hasDestroyedBlock);
  //vtable index: 4
  virtual void stopDestroyBlock(BlockPos const &pos);
  //vtable index: 5
  virtual void startBuildBlock(BlockPos const &pos, uchar face);
  //vtable index: 6
  virtual bool buildBlock(BlockPos const &pos, uchar face);
  //vtable index: 7
  virtual void continueBuildBlock(BlockPos const &pos, uchar face);
  //vtable index: 8
  virtual void stopBuildBlock();
  //vtable index: 9
  virtual void tick();
  //vtable index: 10
  virtual float getPickRange(InputMode const &currentInputMode, bool isVR);
  //vtable index: 11
  virtual bool useItem(ItemStack &item);
  //vtable index: 12
  virtual InteractionResult useItemOn(ItemStack &item, BlockPos const &at, uchar face,
                                      Vec3 const &hit, Block const *targetBlock, bool isFirstEvent);
  //vtable index: 13
  virtual bool interact(Actor &entity, Vec3 const &location);
  //vtable index: 14
  virtual bool attack(Actor &entity);
  //vtable index: 15
  virtual void releaseUsingItem();
  //vtable index: 16
  virtual void setTrialMode(bool isEnabled);
  //vtable index: 17
  virtual bool isInTrialMode();
  //vtable index: 18
  virtual void registerUpsellScreenCallback(std::function<void(bool)> callback);
};
static_assert(sizeof(GameMode) == 0xB8);