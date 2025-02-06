#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "Helper/Rotation/rotation.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "game/minecraft/network/Packet/Packets/PlayerAuthInputPacket.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},  {"shortcut", false},       {"staircaseMode", false},
    {"rotation", false}, {"rotationSlient", false},
};
static float targetY = 0.5F;
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("staircaseMode", "楼梯模式");
    module->getGUI().CheckBox("rotation", "转头");
    module->getGUI().CheckBox("rotationSlient", "静音转头");
  });
  setOnRender([](Module *module) {
    try {
      bool staircaseMode = module->getGUI().Get<bool>("staircaseMode");
      bool rotation = module->getGUI().Get<bool>("rotation");
      bool rotationSlient = module->getGUI().Get<bool>("rotationSlient");
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
      if (!rotationSlient && rotation) {
        if (speed > 0.05f) {
          glm::vec3 pos = player->getPosition();
          glm::vec3 BlockBelow = pos;
          BlockBelow.y -= 0.5f;
          Helper::Rotation::Rotation rot = Helper::Rotation::toRotation(pos, BlockBelow);
          player->setPitch(83.0f);
          player->setYaw(rot.yaw);
        }
      }
      if (staircaseMode) {
        glm::vec3 blockBelow = player->getEyesPos();
        blockBelow.y -= 3.0f;
        glm::vec3 blockBelowBelow = player->getEyesPos();
        blockBelowBelow.y -= 4.0f;
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
        glm::vec3 blockBelow = player->getEyesPos();
        blockBelow.y -= 3.0f;
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
  setOnSendPacket([](Module *module, Packet *packet) {
    try {
      bool rotation = g_md->getGUI().Get<bool>("rotation");
      bool rotationSlient = g_md->getGUI().Get<bool>("rotationSlient");
      if (!rotation) {
        return true;
      }
      if (!rotationSlient) {
        return true;
      }
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return true;
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return true;
      ItemStack *item = player->getSelectedItem();
      if (!item || !item->isBlock())
        return true;
      glm::vec3 vel = player->getMotion();
      float speed = glm::length(glm::vec2(vel.x, vel.z));
      if (speed < 0.05f) {
        return true;
      }
      glm::vec3 pos = player->getPosition();
      glm::vec3 BlockBelow = pos;
      BlockBelow.y -= 0.5f;
      Helper::Rotation::Rotation rot = Helper::Rotation::toRotation(pos, BlockBelow);
      if (packet->getName() == "MovePlayerPacket") {
        auto *movePacket = static_cast<MovePlayerPacket *>(packet);
        movePacket->mRot = glm::vec2(83.0f, rot.yaw);
        movePacket->mYHeadRot = rot.yaw;
      } else if (packet->getName() == "PlayerAuthInputPacket") {
        auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
        authPacket->mRot = glm::vec2(83.0f, rot.yaw);
        authPacket->mYHeadRot = rot.yaw;
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}
