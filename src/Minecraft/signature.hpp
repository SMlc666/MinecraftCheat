#pragma once
#include <unordered_map>
#include <string>
extern std::unordered_map<std::string, std::string> IDApattern;
extern std::unordered_map<std::string, void *> signatures;
void signaturesInit();