#include "hooks.hpp"
#include "MemTool.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
MemTool::Hook Minecraft_update_;
int Minecraft_update(void *Minecraft) {
  int ret = Minecraft_update_.call<int>(Minecraft);
  ModuleManager::tickAllModules();
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ =
      MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update), nullptr, false);
}