#include "runtimes.hpp"
#include "game/minecraft/network/LoopbackPacketSender.hpp"
class ClientInstance;
static ClientInstance *instance = nullptr;
static LoopbackPacketSender *sender = nullptr;
ClientInstance *runtimes::getClientInstance() {
  return instance;
}
void runtimes::setClientInstance(ClientInstance *minstance) {
  instance = minstance;
}
LoopbackPacketSender *runtimes::getPacketSender() {
  return sender;
}
void runtimes::setPacketSender(LoopbackPacketSender *msender) {
  sender = msender;
}