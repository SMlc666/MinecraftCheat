#include <iostream>
#include "example.hpp"
// 定义一个模块
MyModule::MyModule(const std::string &name, MenuType type) : Module(name, type) {
  setOnEnable([](Module *self) { std::cout << "Enable " << self->getName() << std::endl; });
  setOnDisable([](Module *self) { std::cout << "Disable " << self->getName() << std::endl; });
  setOnTick([](Module *self) { std::cout << "Tick " << self->getName() << std::endl; });
}
int main() {
  MyModule module("MyModule", MenuType::MAIN_MENU);
  ModuleManager::addModule(&module);

  ModuleManager::enableModuleByName("MyModule");
  ModuleManager::tickAllModules();
  ModuleManager::disableModuleByName("MyModule");

  return 0;
}