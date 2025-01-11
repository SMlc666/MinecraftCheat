#pragma once
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
namespace ScriptAPI::Mem {
enum class Type {
  BYTE,   //std::byte
  XOR,    //std::uint8_t
  WORD,   //std::uint16_t
  DWORD,  //std::uint32_t
  FLOAT,  //float
  QWORD,  //std::uint64_t
  DOUBLE, //double
};
extern std::unordered_map<Type, std::string> TypeNamesOnLua;
std::string getProcessName();
luabridge::LuaRef getValue(uintptr_t address, int type, lua_State *L);
uintptr_t getModuleBase(std::string modulename);
uintptr_t getModuleEnd(std::string modulename);
void setValue(uintptr_t address, int type, luabridge::LuaRef value);
} // namespace ScriptAPI::Mem
