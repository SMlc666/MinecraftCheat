#include "MemTool.hpp"
#include "Dobby/dobby.h"
#include "KittyMemory/KittyMemory.hpp"
#include "log.hpp"
#include <string>
#include <unordered_map>
std::unordered_map<void *, bool> g_hooked_funcs;
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
  if (orig_func != nullptr && (!is_destoryed)) {
    return;
  }
  MemTool::destoryHookByAddress(hook_func);
  is_destoryed = true;
  g_log_tool.message(LogLevel::INFO, "Hook", "destroy hook");
}
void *MemTool::findSymbol(const char *moduleName, const char *symbolName) {
  if (symbolName == nullptr) {
    throw std::invalid_argument("symbolName is null");
  }
  return DobbySymbolResolver(moduleName, symbolName);
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