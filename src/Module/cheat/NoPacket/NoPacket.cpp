#include "NoPacket.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <any>
#include <unordered_map>
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
};
cheat::NoPacket::NoPacket() : Module("NoPacket", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnSendPacket([](Module *module, Packet *packet) { return false; });
}