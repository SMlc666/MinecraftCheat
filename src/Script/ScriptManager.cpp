#include <filesystem>
#include "ScriptManager.hpp"
#include <mutex>
#include <vector>
namespace ScriptManager {
std::vector<ScriptInfo> Scripts = {};
std::mutex ScriptsMutex;
void addScript(const std::string &name) {
}
void reloadScripts(const std::filesystem::path &path) {
  std::lock_guard<std::mutex> lock(ScriptsMutex);
  Scripts.clear();
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry.path())) {
      ScriptInfo info(entry.path().string());
      Scripts.emplace_back(info);
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