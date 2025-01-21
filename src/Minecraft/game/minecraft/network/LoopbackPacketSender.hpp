#pragma once
#include "PacketSender.hpp"
//NOLINTBEGIN
class LoopbackPacketSender : public PacketSender {
public:
  virtual ~LoopbackPacketSender() override;
  virtual void send(Packet &) override;
  virtual void sendToServer(Packet &) override;
  virtual void sendToClient(NetworkIdentifier const &, Packet const &, SubClientId) override;
  virtual void sendToClient(UserEntityIdentifierComponent const *, Packet &) override;
  virtual void sendToClients(std::vector<NetworkIdentifierWithSubId> const &, Packet &) override;
  virtual void sendBroadcast(NetworkIdentifier const &, SubClientId, Packet const &) override;
  virtual void sendBroadcast(Packet const &) override;
  virtual void flush(NetworkIdentifier const &, std::function<void(void)> &&) override;
};
//NOLINTEND