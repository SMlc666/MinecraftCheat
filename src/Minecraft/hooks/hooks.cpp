#include "hooks.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include "runtimes/runtimes.hpp"
#include "signature.hpp"
#include <cstddef>
#include <format>
#include <string>
class ClientInstance;
class Level;
class Dimension;
struct LevelVtbl;
struct DimensionVtbl;
struct ResourcePackManager;
struct BlockComponentFactory;
struct BlockDefinitionGroup;
MemTool::Hook ClientInstance_onStartJoinGame_;
MemTool::Hook Level_Level_;
MemTool::Hook Dimension_Dimension_;

void ClientInstance_onStartJoinGame(ClientInstance *self, bool a1, std::string a2, void *a3) {
  ClientInstance_onStartJoinGame_.call<void>(self, a1, a2, a3);
  runtimes::setClientInstance(self);
  g_log_tool.message(LogLevel::INFO, "ClientInstance_onStartJoinGame",
                     std::format("ClientInstance::onStartJoinGame({:p},{}, {}, {})",
                                 reinterpret_cast<void *>(self), a1, a2, a3));
}
void hooksInit() {
  void *clientInstance = getSign<void *>("ClientInstance::onStartJoinGame");
  //ClientInstance_onStartJoinGame_ = MemTool::Hook(
      clientInstance, reinterpret_cast<void *>(ClientInstance_onStartJoinGame), nullptr, false);
      g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}