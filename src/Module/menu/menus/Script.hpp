#pragma once
#include "ScriptManager.hpp"
#include "imgui/imgui.h"
inline void drawScriptMenu() {
  auto Scripts = ScriptManager::getScripts();
  if (ImGui::Button("Reload Scripts")) {
    ScriptManager::reloadScripts();
  }
  for (auto &script : Scripts) {
    if (ImGui::TreeNodeEx(script->getName().c_str())) {
      ImGui::Text("file: %s", script->getFile().c_str());
      ImGui::TreePop();
    }
  }
}