#include "menu.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
static void drawLog(std::vector<LogEntry> &Logs) {
  for (auto &log : Logs) {
    switch (log.level) {
    case LogLevel::DEBUG:
      ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "[DEBUG] %s %s %s",
                         log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::INFO:
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "[INFO] %s %s %s",
                         log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::WARN:
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WARN] %s %s %s",
                         log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::ERROR:
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[ERROR] %s %s %s",
                         log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::FATAL:
      ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "[FATAL] %s %s %s",
                         log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
      break;
    default:
      ImGui::Text("%s %s %s", log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
    }
  }
}
const std::unordered_map<MenuType, std::function<void()>> MenuFunctions = {
    {MenuType::MAIN_MENU, []() { ImGui::Text("Main Menu"); }},
    {MenuType::COMBAT_MENU, []() { ImGui::Text("Combat Menu"); }},
    {MenuType::LOG_MENU, []() {
       if (ImGui::Button("Clear"))
         g_log_tool.cleanLogs();
       ImGui::SameLine();
       if (ImGui::Button("Save"))
         g_log_tool.SaveToFile();
       ImGui::SameLine();
       if (ImGui::TreeNode("tags")) {
         for (auto &tag : g_log_tool.getTagMap()) {
           if (ImGui::TreeNode(tag.first.c_str())) {
             if (ImGui::Button("Clear"))
               g_log_tool.cleanLogs(tag.first);
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
     }}};