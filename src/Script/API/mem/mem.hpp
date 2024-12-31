#pragma once
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <cstddef>
#include <string>
#include <unordered_map>
namespace ScriptAPI::Mem {
extern std::unordered_map<int, std::string> Type;
std::string getProcessName();
} // namespace ScriptAPI::Mem
