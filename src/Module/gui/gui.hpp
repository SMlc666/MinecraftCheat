#pragma once
#include <string>
#include "ModuleManager.hpp"
class GUI {
public:
  GUI(std::shared_ptr<Module> &m_module);
  void SliderInt(std::string second,std::string Text);
private:
  std::shared_ptr<Module> module;
  std::string first;
  
};