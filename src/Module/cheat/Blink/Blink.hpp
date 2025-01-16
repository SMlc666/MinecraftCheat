#pragma once
#include "MemTool.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <unordered_map>
static bool is_blink = false;
MemTool::Hook send_;
MemTool::Hook sendto_;
ssize_t new_send(int sockfd, const void *buf, size_t len, int flags) {
  if (is_blink) {
    return -1;
  }
  auto ret = send_.call<ssize_t>(sockfd, buf, len, flags);
  return ret;
}
int new_sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
               socklen_t addrlen) {
  if (is_blink) {
    return -1;
  }
  auto ret = sendto_.call<int>(fd, buf, len, flags, dest_addr, addrlen);
  return ret;
}
static const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false}};
class Blink : public Module {
public:
  Blink() : Module("Blink", MenuType::COMBAT_MENU, ConfigData) {
    void *send_addr = MemTool::findSymbol(nullptr, "send");
    send_ = MemTool::Hook(send_addr, reinterpret_cast<void *>(new_send), nullptr, false);
    void *sendto_addr = MemTool::findSymbol(nullptr, "sendto");
    sendto_ = MemTool::Hook(sendto_addr, reinterpret_cast<void *>(new_sendto), nullptr, false);
    setOnEnable([](Module *module) { is_blink = true; });
    setOnDisable([](Module *module) { is_blink = false; });
  }
};