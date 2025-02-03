#pragma once
#include "game/minecraft/network/LoopbackPacketSender.hpp"
class ClientInstance;
namespace runtimes {
ClientInstance *getClientInstance();
void setClientInstance(ClientInstance *minstance);
LoopbackPacketSender *getPacketSender();
void setPacketSender(LoopbackPacketSender *msender);
} // namespace runtimes