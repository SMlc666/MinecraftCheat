#include "shortcut.hpp"
#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include <unordered_map>
ImVec4 disableBackgroundColor = ImVec4(0.0F / 255.0F, 255.0F / 255.0F, 207.0F / 255.0F, 1.0F);
ImVec4 enableBackgroundColor = ImVec4(0.0F / 255.0F, 163.0F / 255.0F, 255.0F / 255.0F, 1.0F);
ImVec4 disableTextColor = ImVec4(0.0F / 255.0F, 0.0F / 255.0F, 0.0F / 255.0F, 1.0F);
ImVec4 enableTextColor = ImVec4(0.0F / 255.0F, 0.0F / 255.0F, 0.0F / 255.0F, 1.0F);
ImVec2 shortcutSize = ImVec2(200.0F, 100.0F);
float shortcutRouding = 5.0F;
void drawAllShortcuts() {
  auto mModules = ModuleManager::getModules();
  for (auto &value : mModules) {
    auto &m = value.second;
    auto &gui = m->getGUI();
    if (gui.Has("shortcut") && gui.Get<bool>("shortcut")) {
      bool mEnabled = gui.Has("enabled") && gui.Get<bool>("enabled");
      ImGui::SetNextWindowSize(shortcutSize);
      ImGui::Begin(m->getName().c_str(), nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
      if (mEnabled) {
        ImGui::PushStyleColor(ImGuiCol_Button, enableBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Text, enableTextColor);
      } else {
        ImGui::PushStyleColor(ImGuiCol_Button, disableBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Text, disableTextColor);
      }
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, shortcutRouding);
      if (ImGui::Button(m->getName().c_str(), shortcutSize)) {
        if (mEnabled) {
          ModuleManager::disableModule(m);
          gui.Set("enabled", false);
        } else {
          ModuleManager::enableModule(m);
          gui.Set("enabled", true);
        }
      }
      ImGui::PopStyleColor(2);
      ImGui::PopStyleVar();
      ImGui::End();
    }
  }
}