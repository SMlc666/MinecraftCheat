#include <filesystem>
#include "ScriptManager.hpp"
#include "log.hpp"
#include <mutex>
#include <format>
#include <vector>
namespace ScriptManager {
std::vector<ScriptInfo> Scripts = {};
std::mutex ScriptsMutex;
void addScript(const std::filesystem::path &path) {
  std::lock_guard<std::mutex> lock(ScriptsMutex);
  ScriptInfo info(path.string());
  g_log_tool.message(LogLevel::INFO, "ScriptManager", std::format("Add script: {}", info.file));
  Scripts.emplace_back(info);
}
void reloadScripts(const std::filesystem::path &path) {
  std::lock_guard<std::mutex> lock(ScriptsMutex);
  Scripts.clear();
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry.path())) {
      addScript(entry.path());
    }
  }
}
const std::vector<ScriptInfo> &getScripts() {
  std::lock_guard<std::mutex> lock(ScriptsMutex);
  return Scripts;
}
} // namespace ScriptManager

void ScriptSetup() {
  std::filesystem::path m_path(NormalScriptPath);
  if (!std::filesystem::exists(m_path)) {
    std::filesystem::create_directory(m_path);
  }
  ScriptManager::reloadScripts(NormalScriptPath);
}