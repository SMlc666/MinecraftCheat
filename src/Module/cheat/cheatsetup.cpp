#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "SpinAttack/SpinAttack.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::SpinAttack);
}