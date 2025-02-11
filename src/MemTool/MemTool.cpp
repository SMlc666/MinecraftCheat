#include "MemTool.hpp"
#include "KittyMemory/KittyMemory.hpp"
#include "ShadowHook/include/shadowhook.h"
#include "log.hpp"
#include <string>
//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)

/**
 * 获取当前进程的名称。
 */
std::string MemTool::getProcessName() {
  return KittyMemory::getProcessName();
}

/**
 * 获取指定模块的映射信息。
 */
KittyMemory::ProcMap MemTool::getModuleMap(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  return Module;
}
/**
 * 销毁钩子并清理资源。
 */
void MemTool::Hook::destroy() {
  if (orig_func != nullptr && (!is_destroyed)) {
    return;
  }
  MemTool::destoryHookByAddress(hook_func);
  is_destroyed = true;
  g_log_tool.message(LogLevel::INFO, "Hook", "destroy hook");
}
void *MemTool::findSymbol(const char *moduleName, const char *symbolName) {
  if (symbolName == nullptr) {
    throw std::invalid_argument("symbolName is null");
  }
  void *handle = shadowhook_dlopen(moduleName);
  if (handle == nullptr) {
    throw std::runtime_error("dlopen failed");
  }
  void *symbol = shadowhook_dlsym(handle, symbolName);
  if (symbol == nullptr) {
    throw std::runtime_error("dlsym failed");
  }
  shadowhook_dlclose(handle);
  return symbol;
}
/**
 * Hook类的析构函数，自动销毁钩子（如果启用）。
 */
MemTool::Hook::~Hook() {
  if (auto_destroy) {
    destroy();
  }
}
//NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)