#include "Disabled.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "log.hpp"
#include "menu/menu.hpp"
#include <unordered_map>
MemTool::Hook xdl_open_;
static const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", true}};
const static std::string xdlName = "libxdl.so";
static void *Disabled_xdl_open(const char *filename, int flags) {
  g_log_tool.message(LogLevel::DEBUG, "xdl",
                     std::format("xdl_open called with filename: {} flags: {}", filename, flags));
  auto *ret = xdl_open_.call<void *>(filename, flags);
  return ret;
}
cheat::Disabled::Disabled() : Module("Disabled", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    MemTool::Hook(MemTool::findSymbol(xdlName.c_str(), "xdl_open"), Disabled_xdl_open, nullptr,
                  false);
  });
}