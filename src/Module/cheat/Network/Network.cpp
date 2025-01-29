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
cheat::Network::Network() : Module("Network", MenuType::DEBUG_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnLoad([](Module *module) {});
}