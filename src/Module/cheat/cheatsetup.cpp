#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "SpinAttack/SpinAttack.hpp"
#include "Blink/Blink.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::SpinAttack);
  ModuleManager::addModule(new cheat::Blink);
}