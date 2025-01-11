#include "hook.hpp"
#include "MemTool.hpp"
ScriptAPI::Mem::Hook::ScriptHook::ScriptHook(luabridge::LuaRef _func, luabridge::LuaRef argv)
    : m_func(_func) {
  if (!_func.isFunction()) {
    throw std::runtime_error("Invalid function");
  }
  if (!argv.isTable()) {
    throw std::runtime_error("Invalid arguments");
  }
}