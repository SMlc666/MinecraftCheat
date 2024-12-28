#include "API_lib.hpp"
#include "API/draw/draw.hpp"
#include "Lua/lua.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "print/print.hpp"
#include <functional>
#include <unordered_map>
static std::unordered_map<int, std::function<void(lua_State *LuaState)>> APIinit{
    {1,
     [](lua_State *LuaState) {

     }},
};
void ScriptAPI::init(lua_State *LuaState, int version) {
  luabridge::getGlobalNamespace(LuaState).addFunction("print", ScriptAPI::print);
  luabridge::getGlobalNamespace(LuaState)
      .beginNamespace("draw")
      .addFunction("text", ScriptAPI::draw::Text)
      .endNamespace();
  try {
    APIinit.at(version)(LuaState);
  } catch (const std::out_of_range &e) {
    throw std::runtime_error("API version not found");
  }
}