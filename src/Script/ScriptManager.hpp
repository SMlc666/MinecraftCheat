#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "Lua/lua.hpp"
#include "menu/menu.hpp"
static const std::string NormalScriptPath = "/sdcard/MinecraftCheat/Scripts";
namespace ScriptManager {
class ScriptException : public std::runtime_error {
public:
  ScriptException(const std::string &message, lua_State *L) : std::runtime_error(message) {
    lua_close(L);
  }
};
class Script {
public:
  Script(std::filesystem::path &m_path);
  ~Script();
  Script(const Script &other) = default;
  Script &operator=(const Script &other) = default;
  Script(Script &&other) noexcept = default;
  Script &operator=(Script &&other) noexcept = default;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] std::string getFile() const;
  [[nodiscard]] MenuType getMenu() const;
  void onDraw() const;

private:
  MenuType menu;
  std::string name;
  std::filesystem::path path;
  lua_State *L;
};
const std::vector<std::shared_ptr<Script>> &getScripts();
void reloadScripts(const std::string &path = NormalScriptPath);
} // namespace ScriptManager

void ScriptSetup();