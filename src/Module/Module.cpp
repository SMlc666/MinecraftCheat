#include "Module.hpp"
#include "game/minecraft/input/MoveInputHandler.hpp"
#include "gui/gui.hpp"
#include "imgui/imgui.h"
#include <functional>
#include <unordered_map>
#include <utility>
Module::Module(std::string name, MenuType type, std::unordered_map<std::string, std::any> configMap)
    : m_name(std::move(name)), m_type(type), m_gui(this, m_configMap),
      m_configMap(std::move(configMap)), m_onTick(nullptr), m_onEnable(nullptr),
      m_onDisable(nullptr), m_onLoad(nullptr), m_onDrawGUI(nullptr) {
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

void Module::setOnDrawGUI(std::function<void(Module *)> func) {
  m_onDrawGUI = std::move(func);
}
void Module::setOnDraw(std::function<void(Module *)> func) {
  m_onDraw = std::move(func);
}
void Module::setOnRender(std::function<void(Module *)> func) {
  m_onRender = std::move(func);
}
void Module::setOnPostRender(std::function<void(Module *)> func) {
  m_onPostRender = std::move(func);
}
void Module::setOnSendPacket(std::function<bool(Module *, Packet *)> func) {
  m_onSendPacket = std::move(func);
}
void Module::setOnMove(std::function<void(Module *, MoveInputHandler *)> func) {
  m_onMove = std::move(func);
}

void Module::onTick() {
  if (!m_onTick) {
    return;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      m_onTick(this);
    }
  } catch (...) {
    return;
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

void Module::onDrawGUI() {
  if (ImGui::TreeNode(m_name.c_str())) {
    if (m_onEnable || m_onDisable) {
      if (m_gui.CheckBox("enabled", "Enabled")) {
        if (m_gui.Get<bool>("enabled")) {
          onEnable();
        } else {
          onDisable();
        }
      }
      m_gui.CheckBox("shortcut", "Shortcut");
    }
    if (m_onDrawGUI) {
      m_onDrawGUI(this);
    }
    ImGui::TreePop();
  }
}
void Module::onDraw() {
  if (!m_onDraw) {
    return;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      m_onDraw(this);
    }
  } catch (...) {
    return;
  }
}

void Module::onRender() {
  if (!m_onRender) {
    return;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      m_onRender(this);
    }
  } catch (...) {
    return;
  }
}

void Module::onPostRender() {
  if (!m_onPostRender) {
    return;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      m_onPostRender(this);
    }
  } catch (...) {
    return;
  }
}

bool Module::onSendPacket(Packet *packet) {
  if (!m_onSendPacket) {
    return true;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      return m_onSendPacket(this, packet);
    }
  } catch (...) {
    return true;
  }
  return true;
}
void Module::onMove(MoveInputHandler *inputHandler) {
  if (!m_onMove) {
    return;
  }
  try {
    bool isEnabled = m_gui.Get<bool>("enabled");
    if (isEnabled) {
      m_onMove(this, inputHandler);
    }
  } catch (...) {
    return;
  }
}
GUI &Module::getGUI() {
  return m_gui;
}