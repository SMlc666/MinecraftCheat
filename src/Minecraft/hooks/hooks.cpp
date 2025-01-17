#include "hooks.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
#include <cstddef>
#include <format>
#include <string>
class ClientInstance;
class Minecraft;
MemTool::Hook Minecraft_update_;
MemTool::Hook ClientInstance_ClientInstance_;
bool Minecraft_update(Minecraft *mMinecraft) {
  bool ret = Minecraft_update_.call<bool>(mMinecraft);
  ModuleManager::tickAllModules();
  return ret;
}
ClientInstance *ClientInstance_ClientInstance(ClientInstance *mClientInstance, int a2, int a3,
                                              int a4, char a5, void *a6, void *a7, int a8,
                                              void *a9) {
  auto *ret = ClientInstance_ClientInstance_.call<ClientInstance *>(mClientInstance, a2, a3, a4, a5,
                                                                    a6, a7, a8, a9);
  runtimes::setClientInstance(mClientInstance);
  g_log_tool.message(
      LogLevel::INFO, "ClientInstance_ClientInstance",
      std::format("ClientInstance: {:p}", reinterpret_cast<void *>(mClientInstance)));
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}