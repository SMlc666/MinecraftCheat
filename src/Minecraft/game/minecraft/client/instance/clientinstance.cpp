#include "clientinstance.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "log.hpp"
#include <format>
//NOLINTBEGIN
LocalPlayer *ClientInstance::getLocalPlayer() const {
  using function = LocalPlayer *(*)(const ClientInstance *);
  g_log_tool.message(LogLevel::DEBUG, "ClientInstance::getLocalPlayer",
                     std::format("vtable[33] = {:p}", reinterpret_cast<void *>(this->vtable[33])));
  auto func = reinterpret_cast<function>(this->vtable[33]);
  return func(this);
}

//NOLINTEND