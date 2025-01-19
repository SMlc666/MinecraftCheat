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
Level *Level_Level(Level *self, int a2, void *a3, LevelVtbl *a4, char a5, char a6, LevelVtbl *a7,
                   LevelVtbl **a8, ResourcePackManager *a9, int a10, int a11,
                   BlockComponentFactory **a12, BlockDefinitionGroup **a13, void *a14, int a15,
                   char a16, char a17, std::byte *a18, std::byte *a19) {
  auto *ret = Level_Level_.call<Level *>(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13,
                                         a14, a15, a16, a17, a18, a19);
  g_log_tool.message(LogLevel::INFO, "Level_Level",
                     std::format("Level:{:p}", reinterpret_cast<void *>(self)));
  return ret;
}
Dimension *Dimension_Dimension(Dimension *self, DimensionVtbl *a2, int a3, int a4, int a5,
                               void *a6) {
  auto ret = Dimension_Dimension_.call<Dimension *>(self, a2, a3, a4, a5, a6);
  g_log_tool.message(LogLevel::INFO, "Dimension_Dimension",
                     std::format("Dimension:{:p}", reinterpret_cast<void *>(self)));
  return ret;
}
void hooksInit() {
  void *clientInstance = getSign<void *>("ClientInstance::onStartJoinGame");
  ClientInstance_onStartJoinGame_ = MemTool::Hook(
      clientInstance, reinterpret_cast<void *>(ClientInstance_onStartJoinGame), nullptr, false);
  void *level = getSign<void *>("Level::Level");
  Level_Level_ = MemTool::Hook(level, reinterpret_cast<void *>(Level_Level), nullptr, false);
  void *dimension = getSign<void *>("Dimension::Dimension");
  Dimension_Dimension_ =
      MemTool::Hook(dimension, reinterpret_cast<void *>(Dimension_Dimension), nullptr, false);
  g_log_tool.message(LogLevel::INFO, "HooksInit", "Hooks inited");
}