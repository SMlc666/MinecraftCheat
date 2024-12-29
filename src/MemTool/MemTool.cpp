#include "MemTool.hpp"
#include "Dobby/dobby.h"
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/KittyScanner.hpp"
#include <string>
#include <unordered_map>
static std::unordered_map<void *, bool> g_hooked_funcs;
//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
template <typename T> bool MemTool::write(T address, const void *data, size_t size) {
  return KittyMemory::memWrite(reinterpret_cast<void *>(address), data, size);
}
template <typename T> bool MemTool::read(T address, void *data, size_t size) {
  return KittyMemory::memRead(reinterpret_cast<void *>(address), data, size);
}
template <typename T, typename U> T MemTool::read(U address) {
  T value;
  if (read(address, &value, sizeof(T))) {
    return value;
  }
  return {};
}
template <typename T, typename U> bool MemTool::write(T address, U value) {
  return write(address, &value, sizeof(U));
}
std::string MemTool::getProcessName() {
  return KittyMemory::getProcessName();
}
template <typename T> T MemTool::getModuleBase(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(Module.startAddress);
}
template <typename T> T MemTool::getModuleEnd(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(Module.endAddress);
}
KittyMemory::ProcMap MemTool::getModuleMap(const std::string &moduleName) {
  KittyMemory::ProcMap Module = KittyMemory::getElfBaseMap(moduleName);
  return Module;
}
template <typename T>
T MemTool::findIdaPatternFirst(const std::string &moduleName, const std::string &pattern) {
  KittyMemory::ProcMap Module = getModuleMap(moduleName);
  if (Module.isUnknown()) {
    return {};
  }
  return reinterpret_cast<T>(
      KittyScanner::findIdaPatternFirst(Module.startAddress, Module.endAddress, pattern));
}
template <typename T>
MemTool::Hook::Hook(T address, void *func) : hook_func(reinterpret_cast<void *>(func)) {
  if (g_hooked_funcs.find(reinterpret_cast<void *>(address)) != g_hooked_funcs.end()) {
    throw std::runtime_error("Address already hooked");
  }
  DobbyHook(reinterpret_cast<void *>(address), func, orig_func);
  g_hooked_funcs[reinterpret_cast<void *>(address)] = true;
}
void MemTool::Hook::destroy() {
  if ((orig_func != nullptr) || is_destoryed || (hook_func != nullptr)) {
    return;
  }
  DobbyDestroy(hook_func);
  g_hooked_funcs.erase(hook_func);
  is_destoryed = true;
}
MemTool::Hook::~Hook() {
  destroy();
}
//NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)