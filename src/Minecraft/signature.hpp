#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
extern std::unordered_map<std::string, uintptr_t> IDApattern;
extern std::unordered_map<std::string, void *> signatures;
template <typename T> inline T getSign(const std::string &name) {
  if (signatures.find(name) == signatures.end()) {
    return nullptr;
  }
  return reinterpret_cast<T>(signatures[name]);
}
void signaturesInit();