#include "signature.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include <cstdint>
#include <format>
#include <unordered_map>
static const std::string module = "libminecraftpe.so";
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<std::string, uintptr_t> IDApattern = {
    {"Player::Player", 0x4CE56FC},
};
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<std::string, void *> signatures{};
void signaturesInit() {
  void *base = MemTool::getModuleBase<void *>(module);
  void *end = MemTool::getModuleEnd<void *>(module);
  g_log_tool.message(LogLevel::INFO, "initSignatures",
                     std::format("Module base: {:p}, end: {:p}", base, end));
  for (auto &IDA : IDApattern) {
    void *addr = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(base) + IDA.second);
    signatures.insert({IDA.first, addr});
    g_log_tool.message(LogLevel::INFO, "initSignatures",
                       std::format("Found {} at {:p}", IDA.first, addr));
  }
}