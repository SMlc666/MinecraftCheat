#include "ScriptManager.hpp"
#include <memory>
#include <dirent.h>
#include <sys/stat.h>

Script::Script(std::filesystem::path m_path) : path(m_path) {
  L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, m_path.string().c_str()) != LUA_OK) {
    lua_close(L);
    L = nullptr;
    throw std::runtime_error(std::format("Error loading script: {}", m_path.string()));
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    std::string errorMsg = lua_tostring(L, -1);
    lua_close(L);
    L = nullptr;
    throw std::runtime_error(
        std::format("Error running script: {} - {}", m_path.string(), errorMsg));
  }
  name = path.filename().string();
}
Script::~Script() {
  if (L)
    lua_close(L);
}
std::string Script::getName() const {
  return name;
}
std::string Script::getFile() const {
  return path.string();
}
namespace ScriptManager {
std::vector<std::shared_ptr<Script>> scripts;
const std::vector<std::shared_ptr<Script>> &getScripts() {
  return scripts;
}
void addScript(std::filesystem::path path) {
  scripts.emplace_back(std::make_shared<Script>(path));
}
void clearScripts() {
  scripts.clear();
}
void reloadScripts(std::string path) {
  clearScripts();
  std::filesystem::path p(path);
  struct stat statbuf;
  if (stat(p.string().c_str(), &statbuf) != 0) {
    throw std::runtime_error(std::format("Error accessing directory: {}", p.string()));
  } //判断是否是文件夹
  for (auto &entry : std::filesystem::directory_iterator(p)) {
    if (entry.is_regular_file() && entry.path().extension() == ".lua") {
      addScript(entry.path());
    }
  }
}
} // namespace ScriptManager
void ScriptSetup() {
  ScriptManager::reloadScripts();
}