#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "menu.hpp"

class Module {
public:
  Module(const std::string &name, MenuType type);
  ~Module() = default;

  std::string getName() const;
  MenuType getMenuType() const;

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

namespace ModuleManager {
void addModule(Module *module);
void enableModuleByName(const std::string &name);
void disableModuleByName(const std::string &name);
void enableModule(Module *module);
void disableModule(Module *module);
void tickAllModules();
void loadAllModules();
void drawAllModules();
} // namespace ModuleManager