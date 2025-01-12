#include "signature.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include <cstdint>
#include <format>
#include <unordered_map>
static const std::string module = "libminecraftpe.so";
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<std::string, uintptr_t> IDApattern = {
    {"Player::Player", 0x6DD37FC},
    {"Minecraft::Minecraft", 0x7493638},
    {"Minecraft::update", 0x749472C},
    {"ClientInstance::ClientInstance", 0x509BE10},
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