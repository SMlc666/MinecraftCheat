#include "Scaffold.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "Helper/Block/block.hpp"
#include "glm/fwd.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false},
                                                                     {"shortcut", false}};
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnRender([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }

    LocalPlayer *player = instance->getLocalPlayer();
    if ((player == nullptr) || player->getMotion().y <= 0) {
      return;
    }

    ItemStack *item = player->getSelectedItem();
    if ((item == nullptr) || !item->isBlock()) {
      return;
    }

    glm::vec3 pos = player->getPosition();
    auto canBuildBlocks = Helper::Block::getCanBuildBlocks(player, pos);
    if (canBuildBlocks.empty()) {
      return;
    }
  });
}
