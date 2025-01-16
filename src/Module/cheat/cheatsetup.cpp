#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "Timer/timer.hpp"
#include "Blink/Blink.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::Timer);
  ModuleManager::addModule(new cheat::Blink);
}