#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "Blink/Blink.hpp"
#include "SpinAttack/SpinAttack.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::Blink);
  ModuleManager::addModule(new cheat::SpinAttack);
}