#include "ModuleManager.hpp"
#include "cheat/KillAura/KillAura.hpp"
#include "menu/menu.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>


namespace ModuleManager {
std::unordered_map<std::string, std::shared_ptr<Module>> modules;
std::mutex moduleMutex; // 用于线程安全

void addModule(std::shared_ptr<Module> module) { //NOLINT
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  modules[module->getName()] = module;
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

void enableModule(std::shared_ptr<Module> &module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  if (module) {
    module->onEnable();
  }
}

void disableModule(std::shared_ptr<Module> &module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  if (module) {
    module->onDisable();
  }
}

void tickAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second) {
      pair.second->onTick();
    }
  }
}

void loadAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    if (pair.second) {
      pair.second->onLoad();
    }
  }
}
std::unordered_map<std::string, std::shared_ptr<Module>> &getModules() {
  return modules;
}
} // namespace ModuleManager

void moduleSetup() {
  ModuleManager::addModule(std::make_shared<KillAuraModule>("KillAura", MenuType::MAIN_MENU));
  ModuleManager::loadAllModules();
}