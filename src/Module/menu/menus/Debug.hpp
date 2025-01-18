#pragma once
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "imgui/imgui.h"
#include "runtimes/runtimes.hpp"
#include "log.hpp"
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
    if (ImGui::Button("get localplayer")) {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance != nullptr) {
        auto *player = instance->getLocalPlayer();
        if (player != nullptr) {
          g_log_tool.message(LogLevel::DEBUG, "Debug",
                             std::format("LocalPlayer: {:p}", reinterpret_cast<void *>(player)));
        } else {
          g_log_tool.message(LogLevel::DEBUG, "Debug", "LocalPlayer: nullptr");
        }
      } else {
        g_log_tool.message(LogLevel::DEBUG, "Debug", "ClientInstance: nullptr");
      }
    }
  }
}