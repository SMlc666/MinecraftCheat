#include "Blink.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstddef>
#include <sys/socket.h>
cheat::Blink::Blink() : Module("Blink", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {});
  setOnEnable([](Module *module) { isBlink = true; });
  setOnDisable([](Module *module) { isBlink = false; });
  setOnSendPacket([](Module *module, Packet *packet) {
    if (packet->getName() == "PlayerAuthInputPacket") {
      return false;
    }
  });
}