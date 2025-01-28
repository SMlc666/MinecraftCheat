#include "hooks.hpp"
#include "MemTool.hpp"
#include "ModuleManager.hpp"
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <cstddef>
#include <format>
#include "game/minecraft/client/instance/clientinstance.hpp"
#include <string>
MemTool::Hook ClientInstance_onStartJoinGame_;
MemTool::Hook LocalPlayer_NormalTick_;
int64 ClientInstance_onStartJoinGame(ClientInstance *self, char a1, uint8 *a2, uint a3) {
  auto ret = ClientInstance_onStartJoinGame_.call<int64>(self, a1, a2, a3);
  runtimes::setClientInstance(self);
  g_log_tool.message(LogLevel::INFO, "ClientInstance_onStartJoinGame",
                     std::format("ClientInstance::onStartJoinGame({:p},{}, {}, {})",
                                 reinterpret_cast<void *>(self), a1, reinterpret_cast<void *>(a2),
                                 a3));
  return ret;
}
void LocalPlayer_NormalTick(LocalPlayer *self) {
  LocalPlayer_NormalTick_.call<void>(self);
  ModuleManager::tickAllModules();
}
void hooksInit() {
  {
    void *clientInstance = getSign<void *>("ClientInstance::onStartJoinGame");
    ClientInstance_onStartJoinGame_ = MemTool::Hook(
        clientInstance, reinterpret_cast<void *>(ClientInstance_onStartJoinGame), nullptr, false);
  }
  {
    void *localPlayer = getSign<void *>("LocalPlayer::NormalTick");
    LocalPlayer_NormalTick_ = MemTool::Hook(
        localPlayer, reinterpret_cast<void *>(LocalPlayer_NormalTick), nullptr, false);
  }
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}