#include "AnimatePacket.hpp"
#include "game/minecraft/network/Packet/Packets/AnimatePacket.hpp"
#include "signature.hpp"

AnimatePacket::AnimatePacket(Action action, Actor &e) {
  using function = void (*)(AnimatePacket *, Action, Actor &);
  auto func = getSign<function>("AnimatePacket::AnimatePacket");
  func(this, action, e);
}