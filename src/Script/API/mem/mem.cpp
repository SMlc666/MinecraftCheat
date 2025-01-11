#include "mem.hpp"
#include "Lua/lua.hpp"
#include "MemTool.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>
using namespace ScriptAPI;

std::unordered_map<Mem::Type, std::string> Mem::TypeNamesOnLua{
    {Mem::Type::BYTE, "TYPE_BYTE"},     {Mem::Type::XOR, "TYPE_XOR"},
    {Mem::Type::WORD, "TYPE_WORD"},     {Mem::Type::DWORD, "TYPE_DWORD"},
    {Mem::Type::FLOAT, "TYPE_FLOAT"},   {Mem::Type::QWORD, "TYPE_QWORD"},
    {Mem::Type::DOUBLE, "TYPE_DOUBLE"},
};

std::string Mem::getProcessName() {
  return MemTool::getProcessName();
}
uintptr_t Mem::getModuleBase(std::string modulename) {
  return MemTool::getModuleBase<uintptr_t>(modulename);
}
uintptr_t Mem::getModuleEnd(std::string modulename) {
  return MemTool::getModuleEnd<uintptr_t>(modulename);
}
luabridge::LuaRef Mem::getValue(uintptr_t address, int type, lua_State *L) {
  auto memType = static_cast<Mem::Type>(type);
  luabridge::LuaRef value = luabridge::newTable(L);
  value["address"] = address;
  value["type"] = type;
  if (Mem::TypeNamesOnLua.find(memType) == Mem::TypeNamesOnLua.end()) {
    return value;
  }
  switch (memType) {
  case Type::BYTE:
    value["value"] = MemTool::read<std::byte>(address);
    break;
  case Type::XOR:
    value["value"] = MemTool::read<std::uint8_t>(address);
    break;
  case Type::WORD:
    value["value"] = MemTool::read<std::uint16_t>(address);
    break;
  case Type::DWORD:
    value["value"] = MemTool::read<std::uint32_t>(address);
    break;
  case Type::FLOAT:
    value["value"] = MemTool::read<float>(address);
    break;
  case Type::QWORD:
    value["value"] = MemTool::read<std::uint64_t>(address);
    break;
  case Type::DOUBLE:
    value["value"] = MemTool::read<double>(address);
    break;
  }
  return value;
}
void Mem::setValue(uintptr_t address, int type, luabridge::LuaRef value) {
  auto memType = static_cast<Mem::Type>(type);
  if (Mem::TypeNamesOnLua.find(memType) == Mem::TypeNamesOnLua.end()) {
    return;
  }
  switch (memType) {
  case Type::BYTE:
    MemTool::write(address, static_cast<std::byte>(value["value"]));
    break;
  case Type::XOR:
    MemTool::write(address, static_cast<std::uint8_t>(value["value"]));
    break;
  case Type::WORD:
    MemTool::write(address, static_cast<std::uint16_t>(value["value"]));
    break;
  case Type::DWORD:
    MemTool::write(address, static_cast<std::uint32_t>(value["value"]));
    break;
  case Type::FLOAT:
    MemTool::write(address, static_cast<float>(value["value"]));
    break;
  case Type::QWORD:
    MemTool::write(address, static_cast<std::uint64_t>(value["value"]));
    break;
  case Type::DOUBLE:
    MemTool::write(address, static_cast<double>(value["value"]));
    break;
  }
}