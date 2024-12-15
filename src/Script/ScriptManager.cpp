#include <filesystem>
#include "ScriptManager.hpp"
#include "log.hpp"
#include <mutex>
#include <format>
#include <vector>
Script::Script(const std::filesystem::path &m_path)
    : path(m_path), name(m_path.filename().string()) {
  lua_State *L = luaL_newstate();
  if (!L) {
    L = nullptr;
    throw std::runtime_error("Failed to create Lua state");
  }
  luaL_openlibs(L);
  if (luaL_loadfile(L, m_path.string().c_str()) != 0) {
    lua_close(L);
    L = nullptr;
    throw std::runtime_error(std::format("Failed to load script: {}", m_path.string()));
  }
  if (lua_pcall(L, 0, 0, 0) != 0) {
    std::string errorMsg = lua_tostring(L, -1);
    lua_close(L);
    L = nullptr;
    throw std::runtime_error(
        std::format("Failed to execute Lua script: {} Error: {}", m_path.string(), errorMsg));
  }
}
Script::~Script() {
  if (L)
    lua_close(L);
  L = nullptr;
}
std::string Script::getName() const {
  return name;
}
std::string Script::getFilePath() const {
  return path.string();
}

namespace ScriptManager {
std::vector<Script> Scripts = {};
std::mutex ScriptsMutex;
void addScript(const std::filesystem::path &path) {
  try {
    Script newScript(path);
    Scripts.emplace_back(std::move(newScript));
  } catch (const std::exception &e) {
    g_log_tool.message(LogLevel::ERROR, "ScriptManager",
                       std::format("Exception caught: {}", e.what()));
  }
}
void clearScripts() {
  Scripts.clear();
}
void reloadScripts(const std::filesystem::path &path) {
  std::lock_guard<std::mutex> lock(ScriptsMutex);
  clearScripts();
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry.path())) {
      addScript(entry.path());
    }
  }
}
const std::vector<Script> &getScripts() {
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