#include "Blink.hpp"
#include <cstdio>
bool is_blink = false;
MemTool::Hook sendmsg_;
MemTool::Hook sendto_;
MemTool::Hook send_;
const std::unordered_map<std::string, std::any> BlinkConfigData = {{"enabled", false}};
ssize_t new_sendmsg(int sockfd, const struct msghdr *msg, int flags) {
  if (is_blink) {
    return -1;
  }
  auto ret = sendmsg_.call<ssize_t>(sockfd, msg, flags);
  return ret;
}
ssize_t new_sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                   socklen_t addrlen) {
  if (is_blink) {
    return -1;
  }
  auto ret = sendto_.call<ssize_t>(fd, buf, len, flags, dest_addr, addrlen);
  return ret;
}
ssize_t new_send(int sockfd, const void *buf, size_t len, int flags) {
  if (is_blink) {
    return -1;
  }
  auto ret = send_.call<ssize_t>(sockfd, buf, len, flags);
  return ret;
}