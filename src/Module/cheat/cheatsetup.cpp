#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "SpinAttack/SpinAttack.hpp"
#include "Blink/Blink.hpp"
#include "cheat/Disabled/Disabled.hpp"
#include "cheat/Timer/Timer.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::SpinAttack);
  ModuleManager::addModule(new cheat::Blink);
  ModuleManager::addModule(new cheat::Timer);
  ModuleManager::addModule(new cheat::Disabled);
}