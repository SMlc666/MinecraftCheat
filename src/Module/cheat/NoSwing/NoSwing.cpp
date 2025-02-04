#include "NoSwing.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "menu/menu.hpp"
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
};
cheat::NoSwing::NoSwing() : Module("NoSwing", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnSendPacket([](Module *module, Packet *packet) {
    if (packet->getName().find("AnimatePacket") != std::string::npos) {
      return false;
    }
    return true;
  });
}