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
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},        {"shortcut", false}, {"staircaseMode", false}, {"rotation", false},
    {"rotationSlient", false}, {"Tower", false},    {"TowerMotionY", 0.5F},
};
static float targetY = 0.5F;
static bool TowerOver = false;
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("staircaseMode", "楼梯模式");
    if (ImGui::TreeNode("Tower")) {
      module->getGUI().CheckBox("Tower", "塔模式");
      module->getGUI().SliderFloat("TowerMotionY", "塔模式高度", 0.0F, 1.0F);
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Rotation")) {
      module->getGUI().CheckBox("rotation", "转头");
      module->getGUI().CheckBox("rotationSlient", "静音转头");
      ImGui::TreePop();
    }
  });
  setOnRender([](Module *module) {
    try {
      bool staircaseMode = module->getGUI().Get<bool>("staircaseMode");
      bool rotation = module->getGUI().Get<bool>("rotation");
      bool rotationSlient = module->getGUI().Get<bool>("rotationSlient");
      bool Tower = module->getGUI().Get<bool>("Tower");
      float TowerMotionY = module->getGUI().Get<float>("TowerMotionY");
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
      glm::vec3 orig_motion = vel;
      float speed = glm::length(glm::vec2(vel.x, vel.z));
      vel = glm::normalize(vel);
      glm::vec3 pos = player->getPosition();
      glm::vec3 BlockBelow = pos;
      if (Tower) {
        if (vel.y > 0.0f && !TowerOver) {
          player->setMotion(glm::vec3(orig_motion.x, orig_motion.y + TowerMotionY, orig_motion.z));
          TowerOver = true;
        } else if (vel.y < 0.0f && TowerOver) {
          TowerOver = false;
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
        movePacket->mRot = glm::vec2(rot.pitch, rot.yaw);
        movePacket->mYHeadRot = rot.yaw;
      } else if (packet->getName() == "PlayerAuthInputPacket") {
        auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
        authPacket->mRot = glm::vec2(rot.pitch, rot.yaw);
        authPacket->mYHeadRot = rot.yaw;
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}
