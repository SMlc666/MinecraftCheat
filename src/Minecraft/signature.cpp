#include "signature.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include <cstdint>
#include <format>
#include <unordered_map>
static const std::string module = "libminecraftpe.so";
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<std::string, uintptr_t> IDApattern = {
    //? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? 91 ? ? ? D1 ? ? ? B9 ? ? ? D5 ? ? ? F9 ? ? ? F9 ? ? ? F9
    {"Player::Player", 0x4CE56FC},
    //? ? ? D1 ? ? ? 6D ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? 91 ? ? ? D5 ? ? ? F9 F4 03 00 AA ? ? ? F8 ? ? ? 95
    {"Minecraft::update", 0x5E8EBA0},
    //? ? ? D1 ? ? ? FD ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? 91 ? ? ? A9 ? ? ? D5 ? ? ? F9 ? ? ? 90
    {"ClientInstance::ClientInstance", 0x64CE3D0},
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