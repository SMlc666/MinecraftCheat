#include "hooks.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
#include <cstddef>
#include <format>
#include <string>
class Minecraft;
MemTool::Hook Minecraft_update_;
MemTool::Hook Minecraft_Minecraft_;
bool Minecraft_update(long Minecraft) {
  bool ret = Minecraft_update_.call<bool>(Minecraft);
  ModuleManager::tickAllModules();
  return ret;
}
Minecraft *Minecraft_Minecraft(Minecraft *a1, void *a2, void *a3, void *a4, void *a5, void *a6,
                               void *a7, void *a8, void *a9, void *a10, char a11, void *a12,
                               void *a13, void *a14, void *a15, void *a16, void *a17) {
  auto *ret = Minecraft_Minecraft_.call<Minecraft *>(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
                                                     a12, a13, a14, a15, a16, a17);
  g_log_tool.message(LogLevel::INFO, "Minecraft__Minecraft", "Minecraft::Minecraft called");
  g_log_tool.message(LogLevel::INFO, "Minecraft__Minecraft",
                     std::format("{:p}", reinterpret_cast<void *>(ret)));
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
  void *Minecraft = getSign<void *>("Minecraft::Minecraft");
  Minecraft_Minecraft_ =
      MemTool::Hook(Minecraft, reinterpret_cast<void *>(Minecraft_Minecraft), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}