#include "Scaffold.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
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
static float targetY = 0.5F;
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
    bool isOnGround = ActorCollision::isOnGround(player->mEntityContext);
    if (isOnGround || (pos.y - targetY > 0.5F)) {
      targetY = glm::floor(pos.y) - 0.5F;
    }
    glm::vec3 motion = player->getMotion();
    glm::ivec3 targetBlock = {pos.x + motion.x, targetY, pos.z + motion.z};
    if (Helper::Block::isAirBlock(targetBlock)) {
      Helper::Block::predictBlock(targetBlock, &player->getGameMode(), 5.0F);
    }
  });
}
