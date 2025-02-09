#include "ModuleManager.hpp"
#include "cheat/cheatsetup.hpp"
#include "game/minecraft/input/MoveInputHandler.hpp"
#include "log.hpp"
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

namespace ModuleManager {
std::unordered_map<std::string, Module *> modules;
std::mutex moduleMutex; // 用于线程安全

void addModule(Module *module) { //NOLINT
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  modules[module->getName()] = module;
  g_log_tool.message(LogLevel::INFO, "ModuleManager",
                     std::format("Module {} added", module->getName()));
}

void enableModuleByName(const std::string &name) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  auto moduleIterator = modules.find(name);
  if (moduleIterator != modules.end()) {
    moduleIterator->second->onEnable();
  }
}

void disableModuleByName(const std::string &name) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  auto moduleIterator = modules.find(name);
  if (moduleIterator != modules.end()) {
    moduleIterator->second->onDisable();
  }
}

void enableModule(Module *&module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  if (module != nullptr) {
    module->onEnable();
  }
}

void disableModule(Module *&module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  if (module != nullptr) {
    module->onDisable();
  }
}

void tickAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      pair.second->onTick();
    }
  }
}
void renderAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      pair.second->onRender();
    }
  }
}
void postRenderAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      pair.second->onPostRender();
    }
  }
}
void loadAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      pair.second->onLoad();
    }
  }
}
bool sendPacketAllModules(Packet *packet) {
  //std::lock_guard<std::mutex> lockGuard(moduleMutex);
  bool result = true;
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      result &= pair.second->onSendPacket(packet);
    }
  }
  return result;
}
void moveAllModules(MoveInputHandler *inputHandler) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second != nullptr) {
      pair.second->onMove(inputHandler);
    }
  }
}
std::unordered_map<std::string, Module *> &getModules() {
  return modules;
}
} // namespace ModuleManager

void moduleSetup() {
  cheatSetup();
  ModuleManager::loadAllModules();
}