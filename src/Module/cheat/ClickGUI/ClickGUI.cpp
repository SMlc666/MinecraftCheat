#include "ClickGUI.hpp"
#include "draw.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include <thread>
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
    module->getGUI().SliderFloat("WindowRounding", "窗口圆角", 0.0F, 24.0F,
                                 [](float value) { ImGui::GetStyle().WindowRounding = value; });
  });
  setOnLoad([](Module *module) {
    std::thread([module]() {
      while (!is_ImguiSetup) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      if (module->getGUI().Has("WindowRounding")) {
        ImGui::GetStyle().WindowRounding = module->getGUI().Get<float>("WindowRounding");
      }
    }).detach();
  });
}