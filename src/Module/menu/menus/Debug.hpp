#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "imgui/imgui.h"
#include "runtimes/runtimes.hpp"
#include "log.hpp"
#include <cstdlib>
#include <format>
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
  if (ImGui::TreeNode("LocalPlayer")) {
    ClientInstance *instance = runtimes::getClientInstance();
    LocalPlayer *localPlayer{};
    if (instance != nullptr) {
      localPlayer = instance->getLocalPlayer();
    }
    if (ImGui::Button("get localplayer")) {
      if (localPlayer != nullptr) {
        g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                           std::format("LocalPlayer: {:p}", reinterpret_cast<void *>(localPlayer)));
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("get Name")) {
      if (localPlayer != nullptr) {
        g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                           std::format("LocalPlayer Name: {}", localPlayer->getName()));
      }
    }
    ImGui::TreePop();
  }
}