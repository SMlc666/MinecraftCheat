#include "menu.hpp"
#include "imgui/imgui.h"
#include "log.hpp"

const std::unordered_map<MenuType, std::function<void()>> MenuFunctions = {
    {MenuType::MAIN_MENU, []() { ImGui::Text("Main Menu"); }},
    {MenuType::COMBAT_MENU, []() { ImGui::Text("Combat Menu"); }},
    {MenuType::LOG_MENU, []() {
       if (ImGui::Button("Clear"))
         g_log_tool.cleanLogs();
       ImGui::SameLine();
       if (ImGui::Button("Save"))
         g_log_tool.SaveToFile();
       auto Logs = g_log_tool.getLogs();
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
           ImGui::Text("%s %s %s", log.getFormattedTime().c_str(), log.tag.c_str(),
                       log.message.c_str());
         }
       }
     }}};