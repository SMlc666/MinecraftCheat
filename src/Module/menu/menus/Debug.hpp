#pragma once
#include "imgui/imgui.h"
#include <cstdlib>
#include <stdexcept>
void drawDebugMenu() {
  if (ImGui::TreeNode("Crash Test")) {
    if (ImGui::Button("Test Crash Abort")) {
      std::abort();
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash Throw")) {
      throw std::runtime_error("Test Crash Throw");
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash Null Pointer")) {
      int *ptr = nullptr;
      *ptr = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash SIGSEGV")) {
      *(int *)0 = 0; //NOLINT
    }
    ImGui::TreePop();
  }
}