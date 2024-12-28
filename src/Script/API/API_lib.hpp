#pragma once
#include "Lua/lua.h"
namespace ScriptAPI {
void initDefaultAPI(lua_State *LuaState);
void initByVersionAPI(lua_State *LuaState, int version);
} // namespace ScriptAPI