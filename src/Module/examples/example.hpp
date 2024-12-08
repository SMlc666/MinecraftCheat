#include "ModuleManager.hpp"
#include <iostream>
// 定义一个模块
class MyModule : public Module {
public:
  MyModule(const std::string &name, MenuType type);
};