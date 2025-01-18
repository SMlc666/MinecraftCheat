#include "clientinstance.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "log.hpp"
#include <format>
//NOLINTBEGIN
LocalPlayer *ClientInstance::getLocalPlayer() const {
  using function = LocalPlayer *(*)(const ClientInstance *);
  auto func = reinterpret_cast<function>(this->vtable[33]);
  return func(this);
}

//NOLINTEND