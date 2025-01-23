#include "hooks.hpp"
#include "MemTool.hpp"
#include "ModuleManager.hpp"
#include "base/mcint.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <cstddef>
#include <format>
#include <string>
class ClientInstance;
class Minecraft;
MemTool::Hook ClientInstance_onStartJoinGame_;
MemTool::Hook Minecraft_update_;
int64 ClientInstance_onStartJoinGame(ClientInstance *self, char a1, uint8 *a2, uint a3) {
  auto ret = ClientInstance_onStartJoinGame_.call<int64>(self, a1, a2, a3);
  runtimes::setClientInstance(self);
  g_log_tool.message(LogLevel::INFO, "ClientInstance_onStartJoinGame",
                     std::format("ClientInstance::onStartJoinGame({:p},{}, {}, {})",
                                 reinterpret_cast<void *>(self), a1, reinterpret_cast<void *>(a2),
                                 a3));
  return ret;
}
bool Minecraft_update(Minecraft *self) {
  auto ret = Minecraft_update_.call<bool>(self);
  ModuleManager::tickAllModules();
  return ret;
}
void hooksInit() {
  {
    void *clientInstance = getSign<void *>("ClientInstance::onStartJoinGame");
    ClientInstance_onStartJoinGame_ = MemTool::Hook(
        clientInstance, reinterpret_cast<void *>(ClientInstance_onStartJoinGame), nullptr, false);
  }
  {
    void *minecraft = getSign<void *>("Minecraft::update");
    Minecraft_update_ =
        MemTool::Hook(minecraft, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
  }
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}