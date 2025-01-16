#pragma once
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
class Actor;
class Vec3;
class SurvivalMode : public GameMode {
public:
  //NOLINTBEGIN
  //vtable index: 14
  virtual bool attack(Actor &entity); //override
  //vtable index: 13
  virtual bool interact(Actor &entity, Vec3 const &location); //override
  //vtable index: 2
  virtual bool destoryBlock(BlockPos const &pos, uchar face, bool &hasDestroyedBlock); //override
  //vtable index: 1
  virtual bool startDestroyBlock(BlockPos const &pos, uchar face,
                                 bool &hasDestroyedBlock); //override
  //vtable index: 5
  virtual void startBuildBlock(BlockPos const &pos, uchar face); //override
  //vtable index: 6
  virtual bool buildBlock(BlockPos const &pos, uchar face, bool const isSimTick); //override
  //vtable index: 11
  virtual bool useItem(ItemStack &item); //override
  //vtable index: 12
  virtual ::InteractionResult useItemOn(ItemStack &item, BlockPos const &at, uchar face,
                                        Vec3 const &hit, Block const *targetBlock,
                                        bool isFirstEvent); //override
  //vtable index: 9
  virtual void tick(); //override
  //vtable index: 16
  virtual void setTrialMode(bool isEnabled); //override
  //vtable index: 17
  virtual bool isInTrialMode(); //override
  //vtable index: 18
  virtual void registerUpsellScreenCallback(::std::function<void(bool)> callback); //override
  //vtable index: 0
  virtual ~SurvivalMode() = default; //override
  //NOLINTEND
};