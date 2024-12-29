#pragma once
#include "Lua/lua.hpp"
namespace ScriptAPI {
void initDefaultAPI(lua_State *LuaState);
void initByVersionAPI(lua_State *LuaState, int version);
} // namespace ScriptAPI