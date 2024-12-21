#include "ScriptManager.hpp"
#include "API/print/print.hpp"
#include <LuaBridge/LuaBridge.h>
#include "Lua/lua.h"
#include "LuaBridge/detail/Namespace.h"
#include <memory>
#include <dirent.h>
#include <sys/stat.h>
namespace ScriptManager {
Script::Script(std::filesystem::path &m_path) : path(m_path) {
  L = luaL_newstate();
  luaL_openlibs(L);
  luabridge::getGlobalNamespace(L).addFunction("print", ScriptAPI::print);
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
  if (L != nullptr) {
    lua_close(L);
  }
}
std::string Script::getName() const {
  return name;
}
std::string Script::getFile() const {
  return path.string();
}
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
void reloadScripts(const std::string &path) {
  clearScripts();
  std::filesystem::path scriptPath(path);
  struct stat statbuf;
  if (stat(scriptPath.string().c_str(), &statbuf) != 0) {
    throw std::runtime_error(std::format("Error accessing directory: {}", scriptPath.string()));
  } //判断是否是文件夹
  for (const auto &entry : std::filesystem::directory_iterator(scriptPath)) {
    if (entry.is_regular_file() && entry.path().extension() == ".lua") {
      addScript(entry.path());
    }
  }
}
} // namespace ScriptManager
void ScriptSetup() {
  std::filesystem::path scriptsPath(NormalScriptPath);
  if (!std::filesystem::exists(scriptsPath)) {
    std::filesystem::create_directory(scriptsPath);
  }
  ScriptManager::reloadScripts();
}