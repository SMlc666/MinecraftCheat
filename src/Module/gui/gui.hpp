#pragma once
#include <any>
#include <string>
#include <unordered_map>
#include "ModuleManager.hpp"
class GUI {
public:
  GUI(std::shared_ptr<Module> &m_module, const std::unordered_map<std::string, std::any> &m_GUIMap);
  bool SliderInt(std::string &second, std::string &text, int min, int max);

private:
  std::unordered_map<std::string, std::any> GUIMap;
  std::shared_ptr<Module> module;
  std::string first;
};