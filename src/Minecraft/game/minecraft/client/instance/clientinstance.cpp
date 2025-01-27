#include "clientinstance.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
//NOLINTBEGIN
LocalPlayer *ClientInstance::getLocalPlayer() const {
  using function = LocalPlayer *(*)(const ClientInstance *);
  auto func = reinterpret_cast<function>(this->vtable[33]);
  return func(this);
}
BlockSource *ClientInstance::getRegion() const {
  using function = BlockSource *(*)(const ClientInstance *);
  auto func = reinterpret_cast<function>(this->vtable[32]);
  return func(this);
}
//NOLINTEND