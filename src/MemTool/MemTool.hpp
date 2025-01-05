#pragma once
#include "Dobby/dobby.h"
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/KittyScanner.hpp"
#include <cstddef>
#include <cstdio>
#include <string>
//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
extern std::unordered_map<void *, bool> g_hooked_funcs;
namespace MemTool {
// 获取指定模块的进程映射
KittyMemory::ProcMap getModuleMap(const std::string &moduleName);
// 写入数据到指定地址
template <typename T> inline bool write(T address, const void *data, size_t size) {
  return KittyMemory::memWrite(reinterpret_cast<void *>(address), data, size);
}
// 从指定地址读取数据
template <typename T> bool read(T address, void *data, size_t size) {
  return KittyMemory::memRead(reinterpret_cast<void *>(address), data, size);
}
// 从地址读取特定类型的数据
template <typename T, typename U> inline T read(U address) {
  T value;
  if (read(address, &value, sizeof(T))) {
    return value;
  }
  return {};
}
// 写入特定类型的数据到地址
template <typename T, typename U> inline bool write(T address, U value) {
  return write(address, &value, sizeof(U));
}
// 获取指定模块的基地址
template <typename T> inline T getModuleBase(const std::string &moduleName) {
  KittyScanner::ElfScanner Module = KittyScanner::ElfScanner::createWithPath(moduleName);
  uintptr_t base = Module.baseSegment().startAddress;
  return reinterpret_cast<T>(base);
}
// 获取指定模块的结束地址
template <typename T> T inline getModuleEnd(const std::string &moduleName) {
  KittyScanner::ElfScanner Module = KittyScanner::ElfScanner::createWithPath(moduleName);
  uintptr_t end = Module.baseSegment().endAddress;
  return reinterpret_cast<T>(end);
}
// 查找模块中第一个匹配的模式
template <typename T>
inline T findIdaPatternFirst(const std::string &moduleName, const std::string &pattern) {
  KittyScanner::ElfScanner Module = KittyScanner::ElfScanner::createWithPath(moduleName);
  uintptr_t buf = KittyScanner::findIdaPatternFirst(Module.baseSegment().startAddress,
                                                    Module.baseSegment().endAddress, pattern);
  return reinterpret_cast<T>(buf);
}

// 根据符号名称查找模块地址
void *findSymbol(const char *moduleName, const char *symbolName);

// 获取当前进程的名称
std::string getProcessName();
// 销毁指定地址的 Hook
template <typename T> inline void destoryHookByAddress(T address) {
  if (g_hooked_funcs.find(reinterpret_cast<void *>(address)) != g_hooked_funcs.end()) {
    DobbyDestroy(reinterpret_cast<void *>(address));
    g_hooked_funcs.erase(reinterpret_cast<void *>(address));
  }
}
// Hook 类用于实现函数钩子
class Hook {
public:
  // Hook 构造函数
  inline Hook() = default;
  template <typename T>
  inline Hook(T address, void *func, void **m_orig_func, bool m_auto_destroy = true)
      : auto_destroy(m_auto_destroy), hook_func(reinterpret_cast<void *>(func)) {
    if (g_hooked_funcs.find(reinterpret_cast<void *>(address)) != g_hooked_funcs.end()) {
      throw std::runtime_error("Address already hooked");
    }
    void **buf_ptr = nullptr;
    DobbyHook(reinterpret_cast<void *>(address), func, buf_ptr);
    m_orig_func = buf_ptr;
    orig_func = buf_ptr;
    g_hooked_funcs[reinterpret_cast<void *>(address)] = true;
  }
  template <typename T> [[nodiscard]] T original() const {
    return reinterpret_cast<T>(orig_func);
  }
  template <typename RetT = void, typename... Args> RetT call(Args... args) {
    return original<RetT (*)(Args...)>()(args...);
  }
  template <typename RetT = void, typename... Args> RetT ccall(Args... args) {
    return original<RetT(__cdecl *)(Args...)>()(args...);
  }
  // 删除复制构造函数
  Hook(const Hook &) = delete;

  // 删除复制赋值运算符
  Hook &operator=(const Hook &) = default;

  // 删除移动构造函数
  Hook(Hook &&) = delete;

  // 删除移动赋值运算符
  Hook &operator=(Hook &&) = default;

  // Hook 析构函数
  ~Hook();

  // 释放 Hook
  void destroy();

private:
  // 是否自动销毁
  bool auto_destroy = true;

  // 是否已经被销毁
  bool is_destoryed = false;

  // 原始函数指针
  void **orig_func = nullptr;

  // 钩子函数指针
  void *hook_func = nullptr;
};
}; // namespace MemTool
//NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)