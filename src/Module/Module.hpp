#pragma once
#include "game/minecraft/input/MoveInputHandler.hpp"
#include "game/minecraft/network/Packet/Packet.hpp"
#include "menu/menu.hpp"
#include <any>
#include <functional>
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
  void setOnDrawGUI(std::function<void(Module *)> func);
  void setOnDraw(std::function<void(Module *)> func);
  void setOnRender(std::function<void(Module *)> func);
  void setOnPostRender(std::function<void(Module *)> func);
  void setOnSendPacket(std::function<bool(Module *, Packet *)> func);
  void setOnMove(std::function<void(Module *, MoveInputHandler *)> func);
  void onTick();
  void onEnable();
  void onDisable();
  void onLoad();
  void onDrawGUI();
  void onDraw();
  void onRender();
  void onPostRender();
  bool onSendPacket(Packet *packet);
  void onMove(MoveInputHandler *inputHandler);
  GUI &getGUI();

private:
  std::string m_name;
  MenuType m_type;
  std::unordered_map<std::string, std::any> m_configMap;
  std::function<void(Module *)> m_onTick;
  std::function<void(Module *)> m_onEnable;
  std::function<void(Module *)> m_onDisable;
  std::function<void(Module *)> m_onLoad;
  std::function<void(Module *)> m_onDrawGUI;
  std::function<void(Module *)> m_onDraw;
  std::function<void(Module *)> m_onRender;
  std::function<void(Module *)> m_onPostRender;
  std::function<bool(Module *, Packet *)> m_onSendPacket;
  std::function<void(Module *, MoveInputHandler *)> m_onMove;
  GUI m_gui;
};
