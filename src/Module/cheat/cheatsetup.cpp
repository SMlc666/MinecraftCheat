#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "Timer/timer.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::Timer);
}