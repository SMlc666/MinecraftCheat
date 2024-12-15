#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include "cheat/KillAura/KillAura.hpp"
#include "log.hpp"
#include "main.hpp"
#include <mutex>
#include <stdexcept>
static bool isMainMenuActivated = false;
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
  if (m_onEnable || m_onDisable) {
  }
  if (m_onDraw) {
    m_onDraw(this);
  }
}
namespace ModuleManager {
std::unordered_map<std::string, std::shared_ptr<Module>> modules;
std::mutex moduleMutex; // 用于线程安全

void addModule(std::shared_ptr<Module> module) {
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

void enableModule(std::shared_ptr<Module> module) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  if (module) {
    module->onEnable();
  }
}

void disableModule(std::shared_ptr<Module> module) {
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

void drawMenu(MenuType menuType) {
  std::lock_guard<std::mutex> lockGuard(moduleMutex);
  std::string MenuName;
  if (!isMainMenuActivated && menuType != MAIN_MENU) {
    return; //主菜单未激活，其他菜单不绘制
  }
  try {
    MenuName = MenuNames.at(menuType);
  } catch (const std::out_of_range &e) {
    g_log_tool.message(LogLevel::ERROR, "drawMenu", "No such menu type" + std::string(e.what()));
    return; // 防止崩溃
  }
  isMainMenuActivated = ImGui::Begin(MenuName.c_str()); // 使用数组中的字符串作为窗口标题
  try {
    MenuFunctions.at(menuType)(); // 调用对应菜单的绘制函数
  } catch (const std::out_of_range &e) {
    g_log_tool.message(LogLevel::ERROR, "drawMenu",
                       "No such menu function" + std::string(e.what()));
    return; // 防止崩溃
  }
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
  drawMenu(LOG_MENU);
}
} // namespace ModuleManager

void moduleSetup() {
  ModuleManager::addModule(std::make_shared<KillAuraModule>("KillAura", MenuType::MAIN_MENU));
  ModuleManager::loadAllModules();
}