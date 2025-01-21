#pragma once
#include "game/minecraft/network/Compressibility.hpp"
#include <functional>
#include <string>
class NetworkPeer {
public:
  struct NetworkStatus;
  enum class Reliability : int {
    Reliable = 0,
    ReliableOrdered = 1,
    Unreliable = 2,
    UnreliableSequenced = 3,
  };
  enum class DataStatus : int {
    HasData = 0,
    NoData = 1,
    BrokenData = 2,
  };

public:
  virtual ~NetworkPeer();
  virtual void sendPacket(std::string const &, Reliability, Compressibility) = 0;
  virtual DataStatus
  receivePacket(std::string &, std::shared_ptr<std::chrono::steady_clock::time_point> const &) = 0;
  virtual NetworkPeer::NetworkStatus getNetworkStatus() const = 0;
  virtual void update();
  virtual void flush(std::function<void()> &&callback);
  virtual bool isLocal() const;
  virtual bool isEncrypted() const;
};