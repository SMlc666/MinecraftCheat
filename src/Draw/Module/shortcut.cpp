#include "shortcut.hpp"
#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include <unordered_map>
ImVec4 disableBackgroundColor = ImVec4(0.0F / 255.0F, 255.0F / 255.0F, 207.0F / 255.0F, 1.0F);
ImVec4 enableBackgroundColor = ImVec4(0.0F / 255.0F, 163.0F / 255.0F, 255.0F / 255.0F, 1.0F);
ImVec4 disableTextColor = ImVec4(0.0F / 255.0F, 0.0F / 255.0F, 0.0F / 255.0F, 1.0F);
ImVec4 enableTextColor = ImVec4(0.0F / 255.0F, 0.0F / 255.0F, 0.0F / 255.0F, 1.0F);
ImVec2 shortcutSize = ImVec2(200.0F, 100.0F);
ImVec2 shortcutPadding = ImVec2(0.0F, 0.0F);
float shortcutTextScale = 1.0F;
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
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disableBackgroundColor);
      } else {
        ImGui::PushStyleColor(ImGuiCol_Button, disableBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Text, disableTextColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, enableBackgroundColor);
      }
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, shortcutRouding);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, shortcutPadding);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
      ImGui::SetWindowFontScale(shortcutTextScale);
      if (ImGui::Button(m->getName().c_str(), shortcutSize)) {
        if (mEnabled) {
          gui.Set("enabled", false);
          ModuleManager::disableModule(m);
        } else {
          gui.Set("enabled", true);
          ModuleManager::enableModule(m);
        }
      }
      ImGui::SetWindowFontScale(1.0F);
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(3);
      ImGui::End();
    }
  }
}