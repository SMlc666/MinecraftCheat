#include "Module.hpp"
#include "gui/gui.hpp"
#include "imgui/imgui.h"
#include <unordered_map>
#include <utility>
Module::Module(std::string name, MenuType type, std::unordered_map<std::string, std::any> configMap)
    : m_name(std::move(name)), m_type(type), m_gui(this, m_configMap),
      m_configMap(std::move(configMap)), m_onTick(nullptr), m_onEnable(nullptr),
      m_onDisable(nullptr), m_onLoad(nullptr), m_onDraw(nullptr) {
}

std::string Module::getName() const {
  return m_name;
}

MenuType Module::getMenuType() const {
  return m_type;
}

void Module::setOnTick(std::function<void(Module *)> func) {
  m_onTick = std::move(func);
}

void Module::setOnEnable(std::function<void(Module *)> func) {
  m_onEnable = std::move(func);
}

void Module::setOnDisable(std::function<void(Module *)> func) {
  m_onDisable = std::move(func);
}

void Module::setOnLoad(std::function<void(Module *)> func) {
  m_onLoad = std::move(func);
}

void Module::setOnDraw(std::function<void(Module *)> func) {
  m_onDraw = std::move(func);
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
  if (ImGui::TreeNode(m_name.c_str())) {
    if (m_onEnable || m_onDisable) {
      if (m_gui.CheckBox("enabled", "Enabled")) {
        if (m_gui.Get<bool>("enabled")) {
          onEnable();
        } else {
          onDisable();
        }
      }
    }
    if (m_onDraw) {
      m_onDraw(this);
    }
    ImGui::TreePop();
  }
}
GUI &Module::getGUI() {
  return m_gui;
}