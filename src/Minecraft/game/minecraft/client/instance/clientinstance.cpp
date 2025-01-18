#include "clientinstance.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
//NOLINTBEGIN
LocalPlayer *ClientInstance::getLocalPlayer() const {
  using function = LocalPlayer *(*)(const ClientInstance *);
  auto func = reinterpret_cast<function>(this->vtable[30]);
  return func(this);
}
//NOLINTEND