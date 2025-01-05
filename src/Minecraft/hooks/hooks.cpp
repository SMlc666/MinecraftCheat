#include "hooks.hpp"
#include "MemTool.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
MemTool::Hook Minecraft_update_;
long Minecraft_update(long Minecraft) {
  long ret = Minecraft_update_.call<long>(Minecraft);
  ModuleManager::tickAllModules();
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
}