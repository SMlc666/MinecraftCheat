#pragma once
#include <string>
#include "Module.hpp"

namespace ModuleManager {
std::unordered_map<std::string, Module *> &getModules();
void addModule(Module *module);
void enableModuleByName(const std::string &name);
void disableModuleByName(const std::string &name);
void enableModule(Module *&module);
void disableModule(Module *&module);
void tickAllModules();
void loadAllModules();
void renderAllModules();
void postRenderAllModules();
bool sendPacketAllModules(Packet *packet);
void moveAllModules(MoveInputHandler *inputHandler);
} // namespace ModuleManager
void moduleSetup();