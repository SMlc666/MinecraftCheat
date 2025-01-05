#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
extern std::unordered_map<std::string, uintptr_t> IDApattern;
extern std::unordered_map<std::string, void *> signatures;
void signaturesInit();