#include "ClickGUI.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include <unordered_map>
#include "Module.hpp"
#include <any>
#include <string>
static const std::unordered_map<std::string, std::any> ConfigData = {{"enabled", true},
                                                                     {"WindowRounding", 10.0F}};
cheat::ClickGUI::ClickGUI() : Module("ClickGUI", MenuType::RENDER_MENU, ConfigData) {
  setOnDisable([](Module *module) {

  });
  setOnEnable([](Module *module) {

  });
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("WindowRounding", "窗口圆角", 0.0F, 10.0F,
                                 [](float value) { ImGui::GetStyle().WindowRounding = value; });
  });
  setOnLoad([](Module *module) {
    if (module->getGUI().Has("WindowRounding")) {
      ImGui::GetStyle().WindowRounding = module->getGUI().Get<float>("WindowRounding");
    }
  });
}