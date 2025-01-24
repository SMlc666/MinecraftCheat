#include "Blink.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <unordered_map>
#include "MemTool.hpp"
static bool isBlink;
MemTool::Hook BlinkSendMsg_;
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
};

static int Blinksendmsg(int fd, const msghdr *msg, int flags) {
  if (isBlink) {
    return 0;
  }
  return BlinkSendMsg_.call<int>(fd, msg, flags);
}
cheat::Blink::Blink() : Module("Blink", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    BlinkSendMsg_ = MemTool::Hook(MemTool::findSymbol(nullptr, "sendmsg"),
                                  reinterpret_cast<void *>(Blinksendmsg), nullptr, false);
  });
  setOnEnable([](Module *module) { isBlink = true; });
  setOnDisable([](Module *module) { isBlink = false; });
}