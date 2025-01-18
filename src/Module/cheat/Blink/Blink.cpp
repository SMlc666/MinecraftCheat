#include "Blink.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <unordered_map>
#include "MemTool.hpp"
static bool isBlink;
static MemTool::Hook send_;
static int Blink_send(int fd, const void *buf, size_t n, int flags) {
  if (isBlink) {
    return 0;
  }
  return send_.call<int>(fd, buf, n, flags);
}
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
};

cheat::Blink::Blink() : Module("Blink", MenuType::COMBAT_MENU, ConfigData) {
  send_ = MemTool::Hook(MemTool::findSymbol(nullptr, "send"), reinterpret_cast<void *>(Blink_send),
                        nullptr, false);
  setOnEnable([](Module *module) { isBlink = true; });
  setOnDisable([](Module *module) { isBlink = false; });
}