#include "Lua/lua.h"
#include "log.hpp"
#include <sstream>
#include "print.hpp"
static void printTable(lua_State *m_luaState, int index, std::ostringstream &oss,
                       const std::string &indent) {
  lua_pushvalue(m_luaState, index); // 将表复制到栈顶
  lua_pushnil(m_luaState);          // 栈顶放入nil，作为lua_next的初始键

  while (lua_next(m_luaState, -2) != 0) { // lua_next将键值对压入栈中，如果返回0则表示已经遍历完
    // -1是值，-2是键，-3是表
    oss << indent;

    if (lua_isstring(m_luaState, -2) != 0) {
      oss << "\"" << lua_tostring(m_luaState, -2) << "\"";
    } else if (lua_isnumber(m_luaState, -2) != 0) {
      oss << lua_tonumber(m_luaState, -2);
    } else {
      oss << "[not a string or number key]";
    }

    oss << " = ";

    if (lua_isstring(m_luaState, -1) != 0) {
      oss << "\"" << lua_tostring(m_luaState, -1) << "\"";
    } else if (lua_isnumber(m_luaState, -1) != 0) {
      oss << lua_tonumber(m_luaState, -1);
    } else if (lua_isboolean(m_luaState, -1)) {
      oss << (lua_toboolean(m_luaState, -1) != 0 ? "true" : "false");
    } else if (lua_isnil(m_luaState, -1)) {
      oss << "nil";
    } else if (lua_istable(m_luaState, -1)) {
      oss << "{\n";
      printTable(m_luaState, -1, oss, indent + "  "); // 递归调用，增加缩进
      oss << indent << "}";
    } else {
      oss << "[unknown type]";
    }

    lua_pop(m_luaState, 1); // 弹出值，保留键和表以供下一次迭代
    oss << ",\n";
  }

  lua_pop(m_luaState, 1); // 弹出表
}
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
    } else if (lua_istable(m_luaState, i) != 0) {
      oss << "{\n";
      printTable(m_luaState, i, oss, "  "); // 递归调用，增加缩进
      oss << "}";
    } else {
      oss << lua_typename(m_luaState, i);
    }
  }
  g_log_tool.message(LogLevel::INFO, "Script", oss.str());
  return 0;
}
} // namespace ScriptAPI