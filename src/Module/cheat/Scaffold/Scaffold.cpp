#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "Helper/Rotation/rotation.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "game/minecraft/network/Packet/Packets/PlayerAuthInputPacket.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},        {"shortcut", false},      {"placeStrict", false},
    {"SameY", false},          {"DownMode", false}, {"rotation", false},
    {"rotationSlient", false}, {"rotationPitch", 65.0F}, {"Tower", false},
    {"TowerMotionY", 0.5F},    {"debug", false},
};
static bool TowerOver = false;
MemTool::Hook Helper_Block_tryScaffold;
static bool Helper_Block_tryScaffold_(LocalPlayer *player, glm::vec3 blockBelow) {
  auto ret = Helper_Block_tryScaffold.call<bool>(player, blockBelow);
  try {
    if (g_md->getGUI().Get<bool>("debug")) {
      g_log_tool.message(LogLevel::DEBUG, "Scaffold", "tryScaffold : {} {} {} status: {}",
                         blockBelow.x, blockBelow.y, blockBelow.z, ret);
    }
  } catch (...) {
  }
  return ret;
}
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    g_md = module;
    Helper_Block_tryScaffold =
        MemTool::Hook(&Helper::Block::tryScaffold,
                      reinterpret_cast<void *>(Helper_Block_tryScaffold_), nullptr, false);
  });
  setOnEnable([](Module *module) {
    try {
      bool SameY = module->getGUI().Get<bool>("SameY");
      if (SameY) {
      }
    } catch (...) {
    }
  });
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("DownMode", "楼梯模式");
    module->getGUI().CheckBox("placeStrict", "严格放置");
    module->getGUI().CheckBox("SameY", "同高度放置");
    if (ImGui::TreeNode("Tower")) {
      module->getGUI().CheckBox("Tower", "塔模式");
      module->getGUI().SliderFloat("TowerMotionY", "塔模式高度", 0.0F, 1.0F);
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Rotation")) {
      module->getGUI().CheckBox("rotation", "转头");
      module->getGUI().SliderFloat("rotationPitch", "转头角度", 0.0F, 180.0F);
      module->getGUI().CheckBox("rotationSlient", "静音转头");
      ImGui::TreePop();
    }
    module->getGUI().CheckBox("debug", "调试模式");
  });
  setOnRender([](Module *module) {
    try {
      bool DownMode = module->getGUI().Get<bool>("DownMode");
      bool rotation = module->getGUI().Get<bool>("rotation");
      bool rotationSlient = module->getGUI().Get<bool>("rotationSlient");
      bool Tower = module->getGUI().Get<bool>("Tower");
      float TowerMotionY = module->getGUI().Get<float>("TowerMotionY");
      float rotationPitch = module->getGUI().Get<float>("rotationPitch");
      bool placeStrict = module->getGUI().Get<bool>("placeStrict");
      bool SameY = module->getGUI().Get<bool>("SameY");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;

      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;

      ItemStack *item = player->getSelectedItem();
      if (!item || !item->isBlock())
        return;
      glm::vec3 orig_motion = player->getMotion();
      if (Tower) {
        if (orig_motion.y > 0.0f && !TowerOver) {
          player->setMotion(glm::vec3(orig_motion.x, orig_motion.y + TowerMotionY, orig_motion.z));
          TowerOver = true;
        } else if (orig_motion.y < 0.0f && TowerOver) {
          TowerOver = false;
        }
      }
      if (rotation && !rotationSlient) {
        player->setPitch(rotationPitch);
      }
      glm::vec3 pos = player->getPosition();
      glm::vec3 BlockBelow = glm::vec3(pos.x, pos.y - 1.0f, pos.z);
      glm::vec3 vel = glm::normalize(orig_motion);
      float speed = glm::length(glm::vec2(orig_motion.x, orig_motion.z));
      if (speed == 0 && Tower && TowerOver) {
        if (!Helper::Block::tryScaffold(player, BlockBelow, placeStrict)) {
          BlockBelow.y += 1.0f;
          if (!Helper::Block::tryScaffold(player, BlockBelow, placeStrict)) {
            return;
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
        movePacket->mRot = glm::vec2(rot.pitch, movePacket->mRot.y);
      } else if (packet->getName() == "PlayerAuthInputPacket") {
        auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
        authPacket->mRot = glm::vec2(rot.pitch, authPacket->mRot->y);
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}
