#pragma once
#include "config/config.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
inline void drawConfigMenu() {
  if (ImGui::Button("Save")) {
    try {
      Config::saveConfig();
    } catch (const std::exception &e) {
      g_log_tool.message(LogLevel::ERROR, "Config", e.what());
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    try {
      Config::loadConfig();
    } catch (const std::exception &e) {
      g_log_tool.message(LogLevel::ERROR, "Config", e.what());
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Reset")) {
    try {
      Config::initConfig();
    } catch (const std::exception &e) {
      g_log_tool.message(LogLevel::ERROR, "Config", e.what());
    }
  }
  if (ImGui::TreeNode("config")) {
    ImGui::Text("config: %s", Config::getString(true).c_str());
    ImGui::TreePop();
  }
}