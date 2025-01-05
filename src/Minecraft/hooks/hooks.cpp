#include "hooks.hpp"
#include "MemTool.hpp"
#include "signature.hpp"
#include "ModuleManager.hpp"
MemTool::Hook Minecraft_update_;
long (*old_Minecraft_update)(long Minecraft) = nullptr;
long Minecraft_update(long Minecraft) {
  long ret = 0;
  if (old_Minecraft_update != nullptr) {
    ret = old_Minecraft_update(Minecraft);
  }
  ModuleManager::tickAllModules();
  return ret;
}
void hooksInit() {
  void *update = getSign<void *>("Minecraft::update");
  Minecraft_update_ = MemTool::Hook(update, reinterpret_cast<void *>(Minecraft_update),
                                    reinterpret_cast<void **>(old_Minecraft_update), false);
}