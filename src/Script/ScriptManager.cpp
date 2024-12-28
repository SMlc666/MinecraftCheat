#include "ScriptManager.hpp"
#include "Lua/lua.hpp"
#include "log.hpp"
#include <memory>
#include <dirent.h>
#include <stdexcept>
#include <sys/stat.h>
#include "LuaBridge/LuaBridge.h"
#include "API/API_lib.hpp"
#include "menu/menu.hpp"
ScriptManager::Script::Script(std::filesystem::path &m_path) : path(m_path), L(luaL_newstate()) {

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
  luabridge::LuaRef LuaName = luabridge::getGlobal(L, "Name");
  luabridge::LuaRef LuaAPIVersion = luabridge::getGlobal(L, "APIVersion");
  luabridge::LuaRef LuaMenuType = luabridge::getGlobal(L, "MenuType");
  if (!LuaAPIVersion.isNumber()) {
    throw std::runtime_error(std::format("APIVersion is not a number in script: {}", name));
  }
  if (!LuaMenuType.isNumber() && !LuaMenuType.isNil()) {
    throw std::runtime_error(std::format("MenuType is not a number or nil in script: {}", name));
  }
  if (!LuaName.isString()) {
    name = path.filename().string();
  } else {
    name = LuaName.cast<std::string>();
  }
  try {
    ScriptAPI::init(L, LuaAPIVersion.cast<int>());
  } catch (const std::exception &e) {
    throw std::runtime_error(e.what());
  }
  menu = static_cast<MenuType>(LuaMenuType.cast<int>());
}
ScriptManager::Script::~Script() {
  if (L != nullptr) {
    lua_close(L);
  }
}
std::string ScriptManager::Script::getName() const {
  return name;
}
std::string ScriptManager::Script::getFile() const {
  return path.string();
}
void ScriptManager::Script::onDraw() const {
  if (L != nullptr) {
    luabridge::LuaRef m_onDraw = luabridge::getGlobal(L, "onDraw");
    if (!m_onDraw.isFunction()) {
      throw std::runtime_error(std::format("onDraw is not a function in script: {}", name));
    }
    try {
      m_onDraw();
    } catch (const luabridge::LuaException &e) {
      throw std::runtime_error(
          std::format("Error running onDraw function in script: {} - {}", name, e.what()));
    }
  }
}
MenuType ScriptManager::Script::getMenu() const {
  return menu;
}
namespace ScriptManager {
std::vector<std::shared_ptr<Script>> scripts;
const std::vector<std::shared_ptr<Script>> &getScripts() {
  return scripts;
}
void addScript(std::filesystem::path path) {
  try {
    scripts.emplace_back(std::make_shared<Script>(path));
  } catch (const std::exception &e) {
    g_log_tool.message(LogLevel::ERROR, "Script", std::string(e.what()));
  }
}
void clearScripts() {
  scripts.clear();
}
void reloadScripts(const std::string &path) {
  clearScripts();
  std::filesystem::path scriptPath(path);
  struct stat statbuf {};
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