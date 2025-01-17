#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "Blink/Blink.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::Blink);
}