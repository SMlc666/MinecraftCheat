#include "Network.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "log.hpp"
#include "game/minecraft/network/LoopbackPacketSender.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "menu/menu.hpp"
#include "signature.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
};
static bool Logger = false;
static MemTool::Hook LoopbackPacketSender_send_;
static void Network_LoopbackPacketSender_send(LoopbackPacketSender *self, Packet *packet) {
  if (Logger) {
    g_log_tool.message(LogLevel::DEBUG, "send", (*packet).getName());
  }
  LoopbackPacketSender_send_.call<void>(self, packet);
}
cheat::Network::Network() : Module("Network", MenuType::DEBUG_MENU, ConfigData) {
  setOnEnable([](Module *module) { Logger = true; });
  setOnDisable([](Module *module) { Logger = false; });
  setOnLoad([](Module *module) {
    LoopbackPacketSender_send_ =
        MemTool::Hook(getSign<void *>("LoopbackPacketSender::send"),
                      reinterpret_cast<void *>(Network_LoopbackPacketSender_send), nullptr, false);
  });
}