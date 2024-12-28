#include "API_lib.hpp"
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

void ScriptAPI::initDefaultAPI(lua_State *LuaState) {
  luabridge::getGlobalNamespace(LuaState).addFunction("print", ScriptAPI::print);
}
void ScriptAPI::initByVersionAPI(lua_State *LuaState, int version) {
  if (auto it = APIinit.find(version); it == APIinit.end()) {
    throw std::runtime_error("API version not found");
  }
  APIinit.at(version)(LuaState);
}