#include "mem.hpp"
#include "MemTool.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>
using namespace ScriptAPI;
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<int, std::string> ScriptAPI::Mem::Type = {
    {1, "BYTE"},   {64, "DOUBLE"}, {4, "DWORD"}, {32, "QWORD"},
    {16, "FLOAT"}, {8, "XOR"},     {2, "WORD"},
};
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::string Mem::getProcessName() {
  return MemTool::getProcessName();
}