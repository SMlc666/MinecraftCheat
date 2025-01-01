#pragma once
#include "menu/menu.hpp"
class Module {
public:
  Module(const std::string &name, MenuType type);
  ~Module() = default;
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

private:
  std::string m_name;
  MenuType m_type;
  std::function<void(Module *)> m_onTick;
  std::function<void(Module *)> m_onEnable;
  std::function<void(Module *)> m_onDisable;
  std::function<void(Module *)> m_onLoad;
  std::function<void(Module *)> m_onDraw;
};
