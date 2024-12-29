#pragma once
#include "KittyMemory/KittyMemory.hpp"
#include <cstddef>
#include <string>
namespace MemTool {
// 写入数据到指定地址
template <typename T> bool write(T address, const void *data, size_t size);
// 从指定地址读取数据
template <typename T> bool read(T address, void *data, size_t size);
// 从地址读取特定类型的数据
template <typename T, typename U> T read(U address);
// 写入特定类型的数据到地址
template <typename T, typename U> bool write(T address, U value);
// 获取指定模块的基地址
template <typename T> T getModuleBase(const std::string &moduleName);
// 获取指定模块的结束地址
template <typename T> T getModuleEnd(const std::string &moduleName);
// 查找模块中第一个匹配的模式
template <typename T>
T findIdaPatternFirst(const std::string &moduleName, const std::string &pattern);
// 根据符号名称查找模块地址
template <typename T> T findSymbol(const std::string &moduleName, const std::string &symbol);
// 获取指定模块的进程映射
KittyMemory::ProcMap getModuleMap(const std::string &moduleName);
// 获取当前进程的名称
std::string getProcessName();
// 销毁指定地址的 Hook
template <typename T> void destoryHookByAddress(T address);
// Hook 类用于实现函数钩子
class Hook {
public:
  // Hook 构造函数
  template <typename T> Hook(T address, void *func, void **orig_func, bool m_auto_destroy = true);

  // 删除复制构造函数
  Hook(const Hook &) = delete;

  // 删除复制赋值运算符
  Hook &operator=(const Hook &) = delete;

  // 删除移动构造函数
  Hook(Hook &&) = delete;

  // 删除移动赋值运算符
  Hook &operator=(Hook &&) = delete;

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