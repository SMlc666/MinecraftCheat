#include "API_lib.hpp"
#include "API/draw/draw.hpp"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "print/print.hpp"
void ScriptAPI::init(lua_State *LuaState) {
  luabridge::getGlobalNamespace(LuaState).addFunction("print", ScriptAPI::print);
  luabridge::getGlobalNamespace(LuaState)
      .beginNamespace("draw")
      .addFunction("text", ScriptAPI::draw::Text)
      .endNamespace();
      
}