#include "signature.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include <cstdint>
#include <format>
#include <unordered_map>
static const std::string module = "libminecraftpe.so";
std::unordered_map<std::string, std::string> IDApattern = {
    {"Player::Player", "? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? 91 ? ? ? D1 ? "
                       "? ? B9 ? ? ? D5 ? ? ? F9 ? ? ? F9 ? ? ? F9"},
};
std::unordered_map<std::string, void *> signatures{};
void signaturesInit() {
  void *base = MemTool::getModuleBase<void *>(module);
  void *end = MemTool::getModuleEnd<void *>(module);
  g_log_tool.message(LogLevel::INFO, "initSignatures",
                     std::format("Module base: {:p}, end: {:p}", base, end));
  for (auto &item : IDApattern) {
    void *ptr = MemTool::findIdaPatternFirst<void *>(module, item.second);
    if (ptr == nullptr) {
      g_log_tool.message(LogLevel::ERROR, "initSignatures",
                         std::format("Failed to find pattern {}", item.second));
    } else {
      signatures[item.first] = ptr;
      g_log_tool.message(LogLevel::INFO, "initSignatures",
                         std::format("Found pattern {} at {:p}", item.first, ptr));
    }
  }
}