#pragma once
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
namespace ScriptAPI::Mem::Hook {
class ScriptHook {
  ScriptHook(luabridge::LuaRef _func, luabridge::LuaRef argv);

private:
  luabridge::LuaRef m_func;
};
} // namespace ScriptAPI::Mem::Hook