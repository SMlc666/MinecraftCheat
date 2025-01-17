#include "hooks.hpp"
#include "MemTool.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
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
bool Minecraft_update(void *Minecraft) {
  bool ret = Minecraft_update_.call<bool>(Minecraft);
  ModuleManager::tickAllModules();
  return ret;
}

void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}