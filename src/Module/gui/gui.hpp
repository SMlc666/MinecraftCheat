#pragma once
#include <string>
#include "ModuleManager.hpp"
class GUI {
public:
  GUI(std::shared_ptr<Module> &m_module);
  bool SliderInt(std::string &second, std::string &text, int min, int max);

private:
  std::shared_ptr<Module> module;
  std::string first;
};