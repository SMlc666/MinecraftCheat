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
    {"enabled", false}, {"shortcut", false}, {"name", false}, {"id", false},
    {"vaild", false},   {"classify", false}

};
cheat::Network::Network() : Module("Network", MenuType::DEBUG_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnLoad([](Module *module) {});
  setOnSendPacket([](Module *module, Packet *packet) {
    bool name = false;
    bool id = false;
    bool vaild = false;
    bool classify = false;
    try {
      name = module->getGUI().Get<bool>("name");
      id = module->getGUI().Get<bool>("id");
      vaild = module->getGUI().Get<bool>("vaild");
      classify = module->getGUI().Get<bool>("classify");
    } catch (...) {
      return true;
    }
    std::stringstream ss;
    std::string PacketName = packet->getName();
    if (name) {
      ss << "name: " << PacketName << std::endl;
    }
    if (id) {
      ss << "id: " << static_cast<int>(packet->getId()) << std::endl;
    }
    if (vaild) {
      ss << "vaild: " << packet->isValid() << std::endl;
    }
    if (classify) {
      g_log_tool.message(LogLevel::DEBUG, PacketName, ss.str());
    } else {
      g_log_tool.message(LogLevel::DEBUG, "Packet", ss.str());
    }
    return true;
  });
}