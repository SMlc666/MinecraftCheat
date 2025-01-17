#include "hooks.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
#include <cstddef>
#include <format>
#include <string>
class Dimension;
class Minecraft;
MemTool::Hook Minecraft_update_;
bool Minecraft_update(long Minecraft) {
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