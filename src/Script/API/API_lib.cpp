#include "API_lib.hpp"
#include "API/draw/draw.hpp"
#include "API/mem/mem.hpp"
#include "Lua/lua.hpp"
#include "LuaSocket/luasocket.h"
#include "LuaSocket/mime.h"
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/detail/Namespace.h"
#include "print/print.hpp"
#include <functional>
#include <unordered_map>
static const std::unordered_map<int, std::function<void(lua_State *LuaState)>> APIinit{
    {1,
     [](lua_State *LuaState) {
       luaopen_socket_core(LuaState);
       luaopen_mime_core(LuaState);
       luabridge::getGlobalNamespace(LuaState)
           .beginNamespace("draw")
           .addFunction("text", ScriptAPI::draw::Text)
           .beginNamespace("native")
           .addFunction("is_inFrame", ScriptAPI::draw::native::is_inFrame)
           .endNamespace()
           .endNamespace();
       auto MemNamespace = luabridge::getGlobalNamespace(LuaState).beginNamespace("Mem");
       for (const auto &pair : ScriptAPI::Mem::TypeNamesOnLua) {
         MemNamespace.addConstant(pair.second.c_str(), static_cast<int>(pair.first));
       }
       MemNamespace.addFunction("getProcessName", ScriptAPI::Mem::getProcessName)
           .addFunction("getValue", ScriptAPI::Mem::getValue)
           .addFunction("setValue", ScriptAPI::Mem::setValue)
           .addFunction("getModuleBase", ScriptAPI::Mem::getModuleBase)
           .addFunction("getModuleEnd", ScriptAPI::Mem::getModuleEnd)
           .endNamespace();
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