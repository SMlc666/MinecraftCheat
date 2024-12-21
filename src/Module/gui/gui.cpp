#include "gui.hpp"
#include "imgui/imgui.h"
GUI::GUI(std::shared_ptr<Module> &m_module)
    : module(m_module), first(MenuNames.at(m_module->getMenuType())) {
}
bool GUI::SliderInt(std::string &second, std::string &text, int min, int max) {
  //bool active = ImGui::SliderInt(text.c_str(), int *v, min, max);
  //return active;
}