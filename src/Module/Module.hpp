#pragma once
#include "menu/menu.hpp"
#include <any>
#include <unordered_map>
#include "gui/gui.hpp"
class Module {

public:
  Module(std::string name, MenuType type, std::unordered_map<std::string, std::any> configMap);
  virtual ~Module() = default;
  Module(const Module &) = default;
  Module &operator=(const Module &) = default;
  Module(Module &&) noexcept = default;
  Module &operator=(Module &&) noexcept = default;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] MenuType getMenuType() const;

  void setOnTick(std::function<void(Module *)> func);
  void setOnEnable(std::function<void(Module *)> func);
  void setOnDisable(std::function<void(Module *)> func);
  void setOnLoad(std::function<void(Module *)> func);
  void setOnDraw(std::function<void(Module *)> func);
  void onTick();
  void onEnable();
  void onDisable();
  void onLoad();
  void onDraw();
  GUI &getGUI();

private:
  std::string m_name;
  MenuType m_type;
  std::unordered_map<std::string, std::any> m_configMap;
  std::function<void(Module *)> m_onTick;
  std::function<void(Module *)> m_onEnable;
  std::function<void(Module *)> m_onDisable;
  std::function<void(Module *)> m_onLoad;
  std::function<void(Module *)> m_onDraw;
  GUI m_gui;
};
