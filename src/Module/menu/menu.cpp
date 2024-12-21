#include "menu.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "ScriptManager.hpp"
#include "config/config.hpp"
static void drawLog(std::vector<LogEntry> &Logs) {
  for (auto &log : Logs) {
    switch (log.level) {
    case LogLevel::DEBUG:
      ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::INFO:
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::WARN:
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::ERROR:
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    case LogLevel::FATAL:
      ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "[%s] %s %s %s",
                         LogLevelNames.at(log.level).c_str(), log.getFormattedTime().c_str(),
                         log.tag.c_str(), log.message.c_str());
      break;
    default:
      ImGui::Text("%s %s %s", log.getFormattedTime().c_str(), log.tag.c_str(), log.message.c_str());
    }
  }
}
const std::unordered_map<MenuType, std::function<void()>> MenuFunctions = {
    {MenuType::MAIN_MENU, []() { ImGui::Text("Main Menu"); }},
    {MenuType::COMBAT_MENU, []() { ImGui::Text("Combat Menu"); }},
    {MenuType::LOG_MENU,
     []() {
       if (ImGui::Button("Clear"))
         g_log_tool.cleanLogs();
       ImGui::SameLine();
       if (ImGui::Button("Save"))
         g_log_tool.SaveToFile();
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
     }},
    {MenuType::SCRIPT_MENU,
     []() {
       auto Scripts = ScriptManager::getScripts();
       if (ImGui::Button("Reload Scripts"))
         ScriptManager::reloadScripts();
       for (auto &script : Scripts) {
         if (ImGui::TreeNodeEx(script->getName().c_str())) {
           ImGui::Text("file: %s", script->getFile().c_str());
           ImGui::TreePop();
         }
       }
     }},
    {MenuType::CONFIG_MENU, []() {
       if (ImGui::Button("Save")) {
         Config::saveConfig();
       }
       ImGui::SameLine();
       if (ImGui::Button("Load")) {
         Config::loadConfig();
       }
       ImGui::SameLine();
       if (ImGui::Button("Reset")) {
         Config::initConfig();
       }
     }}};