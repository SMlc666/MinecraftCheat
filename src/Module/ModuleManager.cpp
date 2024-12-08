#include "ModuleManager.hpp"
#include <mutex>
#include "imgui.h"
#include "menu.hpp"
#include "cheat/KillAura/KillAura.hpp"
namespace ModuleManager {
std::unordered_map<std::string, Module *> modules;
std::mutex moduleMutex; // 用于线程安全

void addModule(Module *module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  modules[module->getName()] = module;
}

void enableModuleByName(const std::string &name) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  auto it = modules.find(name);
  if (it != modules.end()) {
    it->second->onEnable();
  }
}

void disableModuleByName(const std::string &name) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  auto it = modules.find(name);
  if (it != modules.end()) {
    it->second->onDisable();
  }
}

void enableModule(Module *module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  module->onEnable();
}

void disableModule(Module *module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  module->onDisable();
}

void tickAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    pair.second->onTick();
  }
}

void loadAllModules() {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  for (auto &pair : modules) {
    pair.second->onLoad();
  }
}

void drawMenu(MenuType menuType) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  ImGui::Begin(MenuTypeNames[menuType].c_str()); // 使用数组中的字符串作为窗口标题
  for (const auto &pair : modules) {
    if (pair.second && pair.second->getMenuType() == menuType) {
      pair.second->onDraw();
    }
  }
  ImGui::End();
}
// 主绘制函数，根据需要调用
void drawAllModules() {
  drawMenu(MAIN_MENU);
  // 可以继续添加其他菜单类型的调用
}
} // namespace ModuleManager

Module::Module(const std::string &name, MenuType type)
    : m_name(name), m_type(type), m_onTick(nullptr), m_onEnable(nullptr), m_onDisable(nullptr),
      m_onLoad(nullptr), m_onDraw(nullptr) {
}

std::string Module::getName() const {
  return m_name;
}

MenuType Module::getMenuType() const {
  return m_type;
}

void Module::setOnTick(std::function<void(Module *)> func) {
  m_onTick = func;
}

void Module::setOnEnable(std::function<void(Module *)> func) {
  m_onEnable = func;
}

void Module::setOnDisable(std::function<void(Module *)> func) {
  m_onDisable = func;
}

void Module::setOnLoad(std::function<void(Module *)> func) {
  m_onLoad = func;
}

void Module::setOnDraw(std::function<void(Module *)> func) {
  m_onDraw = func;
}

void Module::onTick() {
  if (m_onTick) {
    m_onTick(this);
  }
}

void Module::onEnable() {
  if (m_onEnable) {
    m_onEnable(this);
  }
}

void Module::onDisable() {
  if (m_onDisable) {
    m_onDisable(this);
  }
}

void Module::onLoad() {
  if (m_onLoad) {
    m_onLoad(this);
  }
}

void Module::onDraw() {
  if (m_onDraw) {
    m_onDraw(this);
  }
}
void moduleSetup() {
  KillAuraModule killaura("KillAura", MenuType::MAIN_MENU);
  ModuleManager::addModule(&killaura);

  ModuleManager::loadAllModules();
}