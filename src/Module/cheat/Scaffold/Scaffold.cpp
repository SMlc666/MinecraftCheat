#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
#include <vector>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"staircaseMode", false}};
static float targetY = 0.5F;
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().CheckBox("staircaseMode", "楼梯模式"); });
  setOnRender([](Module *module) {
    try {
      bool staircaseMode = module->getGUI().Get<bool>("staircaseMode");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;

      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;

      ItemStack *item = player->getSelectedItem();
      if (!item || !item->isBlock())
        return;
      glm::vec3 vel = player->getMotion();
      float speed = glm::length(glm::vec2(vel.x, vel.z));
      vel = glm::normalize(vel);

      if (staircaseMode) {
        glm::vec3 blockBelow = player->getPosition();
        blockBelow.y += 0.3f;
        glm::vec3 blockBelowBelow = player->getPosition();
        blockBelowBelow.y -= 0.2f;
        if (!Helper::Block::tryScaffold(player, blockBelow) &&
            !Helper::Block::tryScaffold(player, blockBelowBelow)) {
          if (speed > 0.05f) { // Are we actually walking?
            blockBelow.z -= vel.z * 0.4f;
            blockBelowBelow.z -= vel.z * 0.4f;
            if (!Helper::Block::tryScaffold(player, blockBelow) &&
                !Helper::Block::tryScaffold(player, blockBelowBelow)) {
              blockBelow.x -= vel.x * 0.4f;
              blockBelowBelow.x -= vel.x * 0.4f;
              if (!Helper::Block::tryScaffold(player, blockBelow) &&
                  !Helper::Block::tryScaffold(player, blockBelowBelow)) {
                blockBelow.z += vel.z;
                blockBelow.x += vel.x;
                blockBelowBelow.z += vel.z;
                blockBelowBelow.x += vel.x;
                Helper::Block::tryScaffold(player, blockBelow);
                Helper::Block::tryScaffold(player, blockBelowBelow);
              }
            }
          }
        }
      } else {
        glm::vec3 blockBelow = player->getPosition();
        blockBelow.y -= 0.4f;
        if (!Helper::Block::tryScaffold(player, blockBelow)) {
          if (speed > 0.05f) { // Are we actually walking?
            blockBelow.z -= vel.z * 0.4f;
            if (!Helper::Block::tryScaffold(player, blockBelow)) {
              blockBelow.x -= vel.x * 0.4f;
              if (!Helper::Block::tryScaffold(player, blockBelow)) {
                blockBelow.z += vel.z;
                blockBelow.x += vel.x;
                Helper::Block::tryScaffold(player, blockBelow);
              }
            }
          }
        }
      }
    } catch (...) {
      return;
    }
  });
}
