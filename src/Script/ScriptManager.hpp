#pragma once
#include <string>
#include <vector>
#include <mutex>
extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}
static const std::string NormalScriptPath = "/sdcard/MinecraftCheat/Scripts";
class Script {
public:
  Script(const std::filesystem::path &m_path);
  ~Script();
  std::string getName() const;
  std::string getFilePath() const;

private:
  std::string name;
  std::filesystem::path path;
  lua_State *L;
};

namespace ScriptManager {
void reloadScripts(const std::filesystem::path &path = NormalScriptPath);
const std::vector<Script> &getScripts();

} // namespace ScriptManager
void ScriptSetup();