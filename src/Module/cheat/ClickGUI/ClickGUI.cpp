#include "ClickGUI.hpp"
#include "Module/shortcut.hpp"
#include "draw.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include <thread>
#include <unordered_map>
#include "Module.hpp"
#include <any>
#include <string>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", true},
    {"WindowRounding", 10.0F},
    {"ScrollbarSize ", 10.0F},
    {"Alpha", 1.0F},
    {"DisabledAlpha", 1.0F},
    {"AntiAliasedFill", false},
    {"AntiAliasedLines", false},
    {"AntiAliasedLinesUseTex", false},
    {"TabBarOverlineSize", 1.0F},
    {"FrameRounding", 3.0F},
    {"shortcut", false},
    {"shortcutSize", GUI::Vec2(200.0F, 100.0F)},
};
cheat::ClickGUI::ClickGUI() : Module("ClickGUI", MenuType::RENDER_MENU, ConfigData) {
  setOnDisable([](Module *module) {});
  setOnEnable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("WindowRounding", "窗口圆角", 0.0F, 48.0F,
                                 [](float value) { ImGui::GetStyle().WindowRounding = value; });
    module->getGUI().SliderFloat("ScrollbarSize ", "滚动条大小", 0.0F, 50.0F,
                                 [](float value) { ImGui::GetStyle().ScrollbarSize = value; });
    module->getGUI().SliderFloat("Alpha", "透明度", 0.01F, 1.0F,
                                 [](float value) { ImGui::GetStyle().Alpha = value; });
    module->getGUI().SliderFloat("DisabledAlpha", "禁用透明度", 0.01F, 1.0F,
                                 [](float value) { ImGui::GetStyle().DisabledAlpha = value; });
    module->getGUI().CheckBox("AntiAliasedFill", "抗锯齿填充",
                              [](bool value) { ImGui::GetStyle().AntiAliasedFill = value; });
    module->getGUI().CheckBox("AntiAliasedLines", "抗锯齿线条",
                              [](bool value) { ImGui::GetStyle().AntiAliasedLines = value; });
    module->getGUI().CheckBox("AntiAliasedLinesUseTex", "抗锯齿线条使用纹理",
                              [](bool value) { ImGui::GetStyle().AntiAliasedLinesUseTex = value; });
    module->getGUI().SliderFloat("TabBarOverlineSize", "标签栏上划线大小", 0.0F, 2.0F,
                                 [](float value) { ImGui::GetStyle().TabBarOverlineSize = value; });
    module->getGUI().SliderFloat("FrameRounding", "边框圆角", 0.0F, 48.0F,
                                 [](float value) { ImGui::GetStyle().FrameRounding = value; });
    if (ImGui::TreeNode("快捷键")) {
      module->getGUI().SliderFloat2("shortcutSize", "快捷键大小", 0.0F, 300.0F,
                                    [](float x, float y) { shortcutSize = ImVec2(x, y); });
      ImGui::TreePop();
    }
  });
  setOnLoad([](Module *module) {
    std::thread([module]() {
      while (!is_ImguiSetup) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      if (module->getGUI().Has("WindowRounding")) {
        ImGui::GetStyle().WindowRounding = module->getGUI().Get<float>("WindowRounding");
      }
      if (module->getGUI().Has("ScrollbarSize ")) {
        ImGui::GetStyle().ScrollbarSize = module->getGUI().Get<float>("ScrollbarSize ");
      }
      if (module->getGUI().Has("Alpha")) {
        ImGui::GetStyle().Alpha = module->getGUI().Get<float>("Alpha");
      }
      if (module->getGUI().Has("DisabledAlpha")) {
        ImGui::GetStyle().DisabledAlpha = module->getGUI().Get<float>("DisabledAlpha");
      }
      if (module->getGUI().Has("AntiAliasedFill")) {
        ImGui::GetStyle().AntiAliasedFill = module->getGUI().Get<bool>("AntiAliasedFill");
      }
      if (module->getGUI().Has("AntiAliasedLines")) {
        ImGui::GetStyle().AntiAliasedLines = module->getGUI().Get<bool>("AntiAliasedLines");
      }
      if (module->getGUI().Has("AntiAliasedLinesUseTex")) {
        ImGui::GetStyle().AntiAliasedLinesUseTex =
            module->getGUI().Get<bool>("AntiAliasedLinesUseTex");
      }
      if (module->getGUI().Has("TabBarOverlineSize")) {
        ImGui::GetStyle().TabBarOverlineSize = module->getGUI().Get<float>("TabBarOverlineSize");
      }
      if (module->getGUI().Has("FrameRounding")) {
        ImGui::GetStyle().FrameRounding = module->getGUI().Get<float>("FrameRounding");
      }
    }).detach();
  });
}