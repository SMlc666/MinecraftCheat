#pragma once
#include "KittyMemory/KittyMemory.hpp"
#include <cstddef>
#include <string>
namespace MemTool {
template <typename T> bool write(T address, const void *data, size_t size);
template <typename T> bool read(T address, void *data, size_t size);
template <typename T, typename U> T read(U address);
template <typename T, typename U> bool write(T address, U value);
template <typename T> T getModuleBase(const std::string &moduleName);
template <typename T> T getModuleEnd(const std::string &moduleName);
template <typename T>
T findIdaPatternFirst(const std::string &moduleName, const std::string &pattern);
KittyMemory::ProcMap getModuleMap(const std::string &moduleName);
std::string getProcessName();
class Hook {
public:
  template <typename T> Hook(T address, void *func);
  Hook(const Hook &) = delete;
  Hook &operator=(const Hook &) = delete;
  Hook(Hook &&) = delete;
  Hook &operator=(Hook &&) = delete;
  ~Hook();
  void destroy();

private:
  bool is_destoryed = false;
  void **orig_func = nullptr;
  void *hook_func = nullptr;
};
}; // namespace MemTool