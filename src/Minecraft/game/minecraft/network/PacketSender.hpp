#pragma once
#include "game/common/SubClientId.hpp"
#include "game/deps/core/Bedrock/NonOwnerPointer.hpp"
#include <functional>
#include <vector>
class Packet;
class NetworkIdentifier;
class UserEntityIdentifierComponent;
class NetworkIdentifierWithSubId;
class PacketSender : public Bedrock::EnableNonOwnerReferences {
public:
  virtual ~PacketSender();
  virtual void send(Packet &);
  virtual void sendToServer(Packet &);
  virtual void sendToClient(NetworkIdentifier const &, Packet const &, SubClientId);
  virtual void sendToClient(UserEntityIdentifierComponent const *, Packet &);
  virtual void sendToClients(std::vector<NetworkIdentifierWithSubId> const &, Packet &);
  virtual void sendBroadcast(NetworkIdentifier const &, SubClientId, Packet const &);
  virtual void sendBroadcast(Packet const &);
  virtual void flush(NetworkIdentifier const &, std::function<void(void)> &&);
};
