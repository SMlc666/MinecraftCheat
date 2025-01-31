#include "Blink.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false},
                                                                     {"shortcut", false}};
cheat::Blink::Blink() : Module("Blink", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {});
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnSendPacket([](Module *module, Packet *packet) {
    if (packet->getName() == "PlayerAuthInputPacket") {
      return false;
    }
  });
}