#include "Blink.hpp"
bool is_blink = false;
MemTool::Hook sendmsg_;
MemTool::Hook sendto_;
MemTool::Hook send_;
const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false}};
int new_sendmsg(int sockfd, const struct msghdr *msg, int flags) {
  if (is_blink) {
    return -1;
  }
  auto ret = sendmsg_.call<int>(sockfd, msg, flags);
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
ssize_t new_send(int sockfd, const void *buf, size_t len, int flags) {
  if (is_blink) {
    return -1;
  }
  auto ret = send_.call<ssize_t>(sockfd, buf, len, flags);
  return ret;
}