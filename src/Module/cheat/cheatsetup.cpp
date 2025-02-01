#include "cheatsetup.hpp"
#include "ModuleManager.hpp"
#include "SpinAttack/SpinAttack.hpp"
#include "Blink/Blink.hpp"
#include "cheat/ClickGUI/ClickGUI.hpp"
#include "cheat/Fly/Fly.hpp"
#include "cheat/GodMode/GodMode.hpp"
#include "cheat/KillAura/KillAura.hpp"
#include "cheat/Network/Network.hpp"
#include "cheat/NoPacket/NoPacket.hpp"
#include "cheat/Scaffold/Scaffold.hpp"
#include "cheat/Seckill/Seckill.hpp"
#include "cheat/Timer/Timer.hpp"
#include "cheat/Velocity/Velocity.hpp"
void cheatSetup() {
  ModuleManager::addModule(new cheat::SpinAttack);
  ModuleManager::addModule(new cheat::Blink);
  ModuleManager::addModule(new cheat::Timer);
  ModuleManager::addModule(new cheat::Network);
  ModuleManager::addModule(new cheat::GodMode);
  ModuleManager::addModule(new cheat::ClickGUI);
  ModuleManager::addModule(new cheat::Seckill);
  ModuleManager::addModule(new cheat::KillAura);
  ModuleManager::addModule(new cheat::Velocity);
  ModuleManager::addModule(new cheat::Scaffold);
  ModuleManager::addModule(new cheat::NoPacket);
  ModuleManager::addModule(new cheat::Fly);
}