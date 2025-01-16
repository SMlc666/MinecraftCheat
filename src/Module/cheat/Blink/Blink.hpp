#pragma once
#include "MemTool.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
extern bool is_blink;
extern const std::unordered_map<std::string, std::any> BlinkConfigData;
extern MemTool::Hook send_;
extern MemTool::Hook sendto_;
extern MemTool::Hook sendmsg_;
ssize_t new_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t new_sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                   socklen_t addrlen);
ssize_t new_sendmsg(int sockfd, const struct msghdr *msg, int flags);

namespace cheat {
class Blink : public Module {
public:
  Blink() : Module("Blink", MenuType::COMBAT_MENU, BlinkConfigData) {
    void *send_addr = MemTool::findSymbol(nullptr, "send");
    send_ = MemTool::Hook(send_addr, reinterpret_cast<void *>(new_send), nullptr, false);
    void *sendto_addr = MemTool::findSymbol(nullptr, "sendto");
    sendto_ = MemTool::Hook(sendto_addr, reinterpret_cast<void *>(new_sendto), nullptr, false);
    void *sendmsg_addr = MemTool::findSymbol(nullptr, "sendmsg");
    sendmsg_ = MemTool::Hook(sendmsg_addr, reinterpret_cast<void *>(new_sendmsg), nullptr, false);
    setOnEnable([](Module *module) { is_blink = true; });
    setOnDisable([](Module *module) { is_blink = false; });
  }
};
} // namespace cheat