#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <filesystem>
#include <memory>
#include "Lua/lua.hpp"
static const std::string NormalScriptPath = "/sdcard/MinecraftCheat/Scripts";
class Script {
public:
  Script(std::filesystem::path m_path);
  ~Script();
  std::string getName() const;
  std::string getFile() const;

private:
  std::string name;
  std::filesystem::path path;
  lua_State *L;
};
namespace ScriptManager {
const std::vector<std::shared_ptr<Script>> &getScripts();
void reloadScripts(std::string path = NormalScriptPath);
} // namespace ScriptManager

void ScriptSetup();