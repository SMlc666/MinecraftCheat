#include "menu.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "ScriptManager.hpp"
#include "config/config.hpp"
#include <cstdlib>
#include <thread>
#include <unwind.h>
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
const std::unordered_map<MenuType, std::function<void()>> MenuFunctions = {
    {MenuType::MAIN_MENU, []() { ImGui::Text("Main Menu"); }},
    {MenuType::COMBAT_MENU, []() { ImGui::Text("Combat Menu"); }},
    {MenuType::LOG_MENU,
     []() {
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
     }},
    {MenuType::SCRIPT_MENU,
     []() {
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
     }},
    {MenuType::CONFIG_MENU,
     []() {
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
     }},
    {MenuType::DEBUG_MENU, []() {
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
     }}};