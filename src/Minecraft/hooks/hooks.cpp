#include "hooks.hpp"
#include "MemTool.hpp"
#include "ModuleManager.hpp"
#include "base/mcint.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/input/MoveInputHandler.hpp"
#include "game/minecraft/network/LoopbackPacketSender.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <cstddef>
#include <format>
#include "game/minecraft/client/instance/clientinstance.hpp"
#include <string>
MemTool::Hook ClientInstance_onStartJoinGame_;
MemTool::Hook LocalPlayer_NormalTick_;
MemTool::Hook LevelRenderer_renderLevel_;
MemTool::Hook LoopbackPacketSender_send_;
MemTool::Hook MoveInputHandler_tick_;
MemTool::Hook GameMode_attack_;
class LoopbackPacketSender;
class Packet;
int64 ClientInstance_onStartJoinGame(ClientInstance *self, char a1, uint8 *a2, uint a3) {
  auto ret = ClientInstance_onStartJoinGame_.call<int64>(self, a1, a2, a3);
  runtimes::setClientInstance(self);
  g_log_tool.message(LogLevel::INFO, "ClientInstance_onStartJoinGame",
                     std::format("ClientInstance::onStartJoinGame({:p},{}, {}, {})",
                                 reinterpret_cast<void *>(self), a1, reinterpret_cast<void *>(a2),
                                 a3));
  return ret;
}
int64 MoveInputHandler_tick(MoveInputHandler *self, int64 a2, int64 a3, int64 a4, int64 a5,
                            int64 a6, int64 a7, int64 a8, int64 a9, void *a10, int64 a11, int64 a12,
                            void *a13, void *a14, void *a15, void *a16, void *a17, void *a18,
                            void *a19, int64 a20, int64 a21, int64 a22) {
  auto ret = MoveInputHandler_tick_.call<int64>(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12,
                                                a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
  ModuleManager::moveAllModules(self);
  return ret;
}
void LocalPlayer_NormalTick(LocalPlayer *self) {
  LocalPlayer_NormalTick_.call<void>(self);
  ModuleManager::tickAllModules();
}
int64 LevelRenderer_renderLevel(int64 a1, int64 a2, int64 a3) {
  ModuleManager::renderAllModules();
  auto ret = LevelRenderer_renderLevel_.call<int64>(a1, a2, a3);
  ModuleManager::postRenderAllModules();
  return ret;
}
void Network_LoopbackPacketSender_send(LoopbackPacketSender *self, Packet *packet) {
  runtimes::setPacketSender(self);
  if (!ModuleManager::sendPacketAllModules(packet)) {
    return;
  }
  LoopbackPacketSender_send_.call<void>(self, packet);
}
bool GameMode_attack(GameMode *self, Actor *entity) {
  auto ret = GameMode_attack_.call<bool>(self, entity);
  ClientInstance *clientInstance = runtimes::getClientInstance();
  if (clientInstance == nullptr) {
    return ret;
  }
  LocalPlayer *localPlayer = clientInstance->getLocalPlayer();
  if (localPlayer == nullptr) {
    return ret;
  }
  if (&localPlayer->getGameMode() == self) {
    ModuleManager::attackAllModules(&GameMode_attack_, entity);
  }
  return ret;
}
void hooksInit() {
  try {
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
    {
      void *levelRenderer = getSign<void *>("LevelRenderer::renderLevel");
      LevelRenderer_renderLevel_ = MemTool::Hook(
          levelRenderer, reinterpret_cast<void *>(LevelRenderer_renderLevel), nullptr, false);
    }
    {
      void *loopbackPacketSender = getSign<void *>("LoopbackPacketSender::send");
      LoopbackPacketSender_send_ = MemTool::Hook(
          loopbackPacketSender, reinterpret_cast<void *>(Network_LoopbackPacketSender_send),
          nullptr, false);
    }
    {
      void *moveInputHandler = getSign<void *>("MoveInputHandler::tick");
      MoveInputHandler_tick_ = MemTool::Hook(
          moveInputHandler, reinterpret_cast<void *>(MoveInputHandler_tick), nullptr, false);
    }
    {
      void *gamemode = getSign<void *>("GameMode::attack");
      GameMode_attack_ =
          MemTool::Hook(gamemode, reinterpret_cast<void *>(GameMode_attack), nullptr, false);
    }
  } catch (const std::exception &e) {
    g_log_tool.message(LogLevel::FATAL, "HooksInit", "Failed to init hooks:{} ", e.what());
    std::terminate();
  }
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}