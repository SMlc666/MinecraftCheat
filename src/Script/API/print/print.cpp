#include "Lua/lua.hpp"
#include "log.hpp"
#include <sstream>
#include "print.hpp"

namespace ScriptAPI {
int print(lua_State *m_luaState) {
  int argCount = lua_gettop(m_luaState);
  std::ostringstream oss;
  for (int i = 1; i <= argCount; i++) {
    if (i > 1) {
      oss << " ";
    }
    if (lua_isstring(m_luaState, i) != 0) {
      oss << lua_tostring(m_luaState, i);
    } else if (lua_isnumber(m_luaState, i) != 0) {
      oss << lua_tonumber(m_luaState, i);
    } else if (lua_isboolean(m_luaState, i) != 0) {
      oss << (lua_toboolean(m_luaState, i) != 0 ? "true" : "false");
    } else if (lua_isnil(m_luaState, i) != 0) {
      oss << "nil";
    } else {
      oss << lua_typename(m_luaState, i);
    }
  }
  g_log_tool.message(LogLevel::INFO, "Script", oss.str());
  return 0;
}
} // namespace ScriptAPI