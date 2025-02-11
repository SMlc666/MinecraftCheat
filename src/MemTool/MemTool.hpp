#pragma once
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/KittyScanner.hpp"
#include "KittyMemory/MemoryPatch.hpp"
#include "ShadowHook/include/shadowhook.h"
#include "log.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <format>
//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
namespace MemTool {
// 定义ASM指令集架构
enum ASM_ARCH {
  ASM_ARM32 = 0,
  ASM_ARM64,
  ASM_x86,
  ASM_x86_64,
};
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
// 写入ASM代码到指定地址
template <typename T>
inline bool writeASM(T address, const std::string &asm_code, ASM_ARCH arch = ASM_ARM64) {
  MemoryPatch patch = MemoryPatch::createWithAsm(reinterpret_cast<uintptr_t>(address),
                                                 static_cast<MP_ASM_ARCH>(arch), asm_code);
  return patch.Modify();
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
  shadowhook_unhook(reinterpret_cast<void *>(address));
}
// Hook 类用于实现函数钩子
class Hook {
public:
  // Hook 构造函数
  inline Hook() = default;

  template <typename T>
  inline Hook(T address, void *func, void **m_orig_func, bool m_auto_destroy = true)
      : auto_destroy(m_auto_destroy), is_destroyed(false), hook_func(func) {
    void *addr = reinterpret_cast<void *>(address);

    g_log_tool.message(LogLevel::INFO, "Hook",
                       std::format("Hooking function at {:p} to {:p} with auto_destroy {}", addr,
                                   func, m_auto_destroy));

    void *buf_ptr = nullptr;
    if (shadowhook_hook_func_addr(addr, func, &buf_ptr) == NULL) {
      throw std::runtime_error("Failed to hook function");
    }

    if (m_orig_func != nullptr) {
      *m_orig_func = buf_ptr;
    }

    orig_func = buf_ptr;
  }

  template <typename RetT, typename... Args>
  inline Hook(RetT (*new_func)(Args...), void **m_orig_func, bool m_auto_destroy = true)
      : Hook(reinterpret_cast<void *>(new_func), reinterpret_cast<void *>(new_func), m_orig_func,
             m_auto_destroy) {
  }

  template <typename T> [[nodiscard]] T original() const {
    if (orig_func == nullptr) {
      throw std::runtime_error("Original function not found");
    }
    return reinterpret_cast<T>(orig_func);
  }

  template <typename RetT = void, typename... Args> RetT call(Args... args) {
    if (orig_func == nullptr) {
      throw std::runtime_error("Original function not found");
    }
    return original<RetT (*)(Args...)>()(args...);
  }

  template <typename RetT = void, typename... Args> RetT ccall(Args... args) {
    if (orig_func == nullptr) {
      throw std::runtime_error("Original function not found");
    }
    return original<RetT(__cdecl *)(Args...)>()(args...);
  }

  // 删除复制构造函数
  Hook(const Hook &) = delete;

  Hook &operator=(const Hook &) = default;

  // 删除移动构造函数
  Hook(Hook &&) = delete;

  Hook &operator=(Hook &&) = default;

  // Hook 析构函数
  ~Hook();

  // 释放 Hook
  void destroy();

private:
  // 是否自动销毁
  bool auto_destroy = true;

  // 是否已经被销毁
  bool is_destroyed = false;

  // 原始函数指针
  void *orig_func = nullptr;

  // 钩子函数指针
  void *hook_func = nullptr;
};

}; // namespace MemTool
//NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)