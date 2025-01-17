#include "hooks.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <cstddef>
#include <format>
#include <string>
class ClientInstance;
MemTool::Hook ClientInstance_ClientInstance_;

ClientInstance *ClientInstance_ClientInstance(ClientInstance *mClientInstance, int a2, int a3,
                                              int a4, char a5, void *a6, void *a7, int a8,
                                              void *a9) {
  auto *ret = ClientInstance_ClientInstance_.call<ClientInstance *>(mClientInstance, a2, a3, a4, a5,
                                                                    a6, a7, a8, a9);
  runtimes::setClientInstance(mClientInstance);
  g_log_tool.message(
      LogLevel::INFO, "ClientInstance_ClientInstance",
      std::format("ClientInstance: {:p}", reinterpret_cast<void *>(mClientInstance)));
  return ret;
}
void hooksInit() {
  void *clientInstance = getSign<void *>("ClientInstance::ClientInstance");
  ClientInstance_ClientInstance_ = MemTool::Hook(
      clientInstance, reinterpret_cast<void *>(ClientInstance_ClientInstance), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}