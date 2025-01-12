#include "hooks.hpp"
#include "MemTool.hpp"
#include "game/minecraft/dimension/dimension.hpp"
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
MemTool::Hook Minecraft_Minecraft_;
MemTool::Hook Dimension_Dimension_;
bool Minecraft_update(long Minecraft) {
  bool ret = Minecraft_update_.call<bool>(Minecraft);
  ModuleManager::tickAllModules();
  return ret;
}
Dimension *Dimension_Dimension(Dimension *self, void *a1, void *a2, void *a3, void *a4,
                               std::string dimensionName) {
  auto *ret = Dimension_Dimension_.call<Dimension *>(self, a1, a2, a3, a4, dimensionName);
  g_log_tool.message(LogLevel::INFO, "Dimension__Dimension",
                     std::format("Dimension::Dimension called, name: {}", dimensionName));
  return ret;
}
Minecraft *Minecraft_Minecraft(Minecraft *a1, void *a2, void *a3, void *a4, void *a5, void *a6,
                               void *a7, void *a8, void *a9, void *a10, void *a11, char a12,
                               void *a13, void *a14, void *a15, void *a16, void *a17) {
  auto *ret = Minecraft_Minecraft_.call<Minecraft *>(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
                                                     a12, a13, a14, a15, a16, a17);
  g_log_tool.message(LogLevel::INFO, "Minecraft__Minecraft", "Minecraft::Minecraft called");
  g_log_tool.message(LogLevel::INFO, "Minecraft__Minecraft",
                     std::format("{:p}", reinterpret_cast<void *>(a1)));
  runtimes::setMinecraftInstance(a1);
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
  void *Minecraft = getSign<void *>("Minecraft::Minecraft");
  Minecraft_Minecraft_ =
      MemTool::Hook(Minecraft, reinterpret_cast<void *>(Minecraft_Minecraft), nullptr, false);
  void *Dimension = getSign<void *>("Dimension::Dimension");
  Dimension_Dimension_ =
      MemTool::Hook(Dimension, reinterpret_cast<void *>(Dimension_Dimension), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}