#pragma once
#include <string>
#include "Module.hpp"

namespace ModuleManager {
std::unordered_map<std::string, std::shared_ptr<Module>> &getModules();
void addModule(std::shared_ptr<Module> module);
void enableModuleByName(const std::string &name);
void disableModuleByName(const std::string &name);
void enableModule(std::shared_ptr<Module> &module);
void disableModule(std::shared_ptr<Module> &module);
void tickAllModules();
void loadAllModules();
} // namespace ModuleManager
void moduleSetup();