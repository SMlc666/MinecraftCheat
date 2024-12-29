#pragma once
#include "imgui/imgui.h"
#include "log.hpp"
#include <thread>
#include <vector>
static void drawLog(std::vector<LogEntry> &Logs) {
  for (auto &log : Logs) {
    switch (log.level) {
    case LogLevel::DEBUG:
      ImGui::TextColored(ImVec4(0.0F, 0.0F, 1.0F, 1.0F), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::INFO:
      ImGui::TextColored(ImVec4(1.0F, 1.0F, 1.0F, 1.0F), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::WARN:
      ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::ERROR:
      ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::FATAL:
      ImGui::TextColored(ImVec4(0.0F, 0.0F, 0.0F, 1.0F), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    default:
      ImGui::Text("%s %s %s", log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
    }
  }
}
void drawLogMenu() {
  if (ImGui::Button("Clear")) {
    g_log_tool.cleanLogs();
  }
  ImGui::SameLine();
  if (ImGui::Button("Save")) {
    std::thread([]() { g_log_tool.SaveToFile(); }).detach(); //线程分离
  }
  if (ImGui::TreeNode("tags")) {
    for (const auto &tag : g_log_tool.getTagMap()) {
      if (ImGui::TreeNode(tag.first.c_str())) {
        if (ImGui::Button("Clear")) {
          g_log_tool.cleanLogs(tag.first);
        }
        auto tagLogs = g_log_tool.getLogs(tag.first);
        drawLog(tagLogs);
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("logs")) {
    auto Logs = g_log_tool.getLogs();
    drawLog(Logs);
    ImGui::TreePop();
  }
}