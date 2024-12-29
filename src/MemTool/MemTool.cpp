#include "MemTool.hpp"
#include "Dobby/dobby.h"
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/KittyScanner.hpp"
#include <string>
#include <unordered_map>
static std::unordered_map<void *, bool> g_hooked_funcs;
//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
/**
 * MemTool类提供对内存读写及其它内存操作的工具函数。
 */
template <typename T> bool MemTool::write(T address, const void *data, size_t size) {
  return KittyMemory::memWrite(reinterpret_cast<void *>(address), data, size);
}

/**
 * 从指定地址读取指定大小的数据。
 */
template <typename T> bool MemTool::read(T address, void *data, size_t size) {
  return KittyMemory::memRead(reinterpret_cast<void *>(address), data, size);
}

/**
 * 从指定地址读取数据并返回。
 */
template <typename T, typename U> T MemTool::read(U address) {
  T value;
  if (read(address, &value, sizeof(T))) {
    return value;
  }
  return {};
}

/**
 * 将值写入指定地址。
 */
template <typename T, typename U> bool MemTool::write(T address, U value) {
  return write(address, &value, sizeof(U));
}

/**
 * 获取当前进程的名称。
 */
std::string MemTool::getProcessName() {
  return KittyMemory::getProcessName();
}

/**
 * 获取指定模块的基地址。
 */
template <typename T> T MemTool::getModuleBase(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(Module.startAddress);
}

/**
 * 获取指定模块的结束地址。
 */
template <typename T> T MemTool::getModuleEnd(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(Module.endAddress);
}

/**
 * 获取指定模块的映射信息。
 */
KittyMemory::ProcMap MemTool::getModuleMap(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  return Module;
}

/**
 * 查找指定模块中符合给定模式的第一个地址。
 */
template <typename T>
T MemTool::findIdaPatternFirst(const std::string &moduleName, const std::string &pattern) {
  KittyMemory::ProcMap Module = getModuleMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(
      KittyScanner::findIdaPatternFirst(Module.startAddress, Module.endAddress, pattern));
}

/**
 * 查找指定模块中的符号地址。
 */
template <typename T>
T MemTool::findSymbol(const std::string *moduleName, const std::string &symbol) {
  return reinterpret_cast<T>(DobbySymbolResolver(moduleName->c_str(), symbol.c_str()));
}

/**
 * 根据地址销毁钩子。
 */
template <typename T> void MemTool::destoryHookByAddress(T address) {
  if (g_hooked_funcs.find(reinterpret_cast<void *>(address)) != g_hooked_funcs.end()) {
    DobbyDestroy(reinterpret_cast<void *>(address));
    g_hooked_funcs.erase(reinterpret_cast<void *>(address));
  }
}

/**
 * Hook类用于处理函数钩子的创建和管理。
 */
template <typename T>
MemTool::Hook::Hook(T address, void *func, void **m_orig_func, bool m_auto_destroy)
    : hook_func(reinterpret_cast<void *>(func)), auto_destroy(m_auto_destroy) {
  if (g_hooked_funcs.find(reinterpret_cast<void *>(address)) != g_hooked_funcs.end()) {
    throw std::runtime_error("Address already hooked");
  }
  DobbyHook(reinterpret_cast<void *>(address), func, m_orig_func);
  orig_func = m_orig_func;
  g_hooked_funcs[reinterpret_cast<void *>(address)] = true;
}

/**
 * 销毁钩子并清理资源。
 */
void MemTool::Hook::destroy() {
  if ((orig_func != nullptr) || is_destoryed || (hook_func != nullptr)) {
    return;
  }
  MemTool::destoryHookByAddress(hook_func);
  is_destoryed = true;
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