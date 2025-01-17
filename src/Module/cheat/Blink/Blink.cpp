#include "Blink.hpp"
#include <cstdio>
MemTool::Hook sendmsg_;
MemTool::Hook sendto_;
MemTool::Hook send_;
const std::unordered_map<std::string, std::any> BlinkConfigData = {{"enabled", false}};
ssize_t new_sendmsg(int sockfd, const struct msghdr *msg, int flags) {
  auto ret = sendmsg_.call<ssize_t>(sockfd, msg, flags);
  return ret;
}
ssize_t new_sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                   socklen_t addrlen) {
  auto ret = sendto_.call<ssize_t>(fd, buf, len, flags, dest_addr, addrlen);
  return ret;
}
ssize_t new_send(int sockfd, const void *buf, size_t len, int flags) {
  auto ret = send_.call<ssize_t>(sockfd, buf, len, flags);
  return ret;
}