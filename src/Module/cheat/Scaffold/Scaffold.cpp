#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "Helper/Rotation/rotation.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "game/minecraft/network/Packet/Packets/PlayerAuthInputPacket.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
#include <random>

static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"placeStrict", false},
    {"SameY", false},
    {"DownMode", false},
    {"rotation", false},
    {"rotationSlient", false},
    {"rotationPitchMin", 60.0F},
    {"rotationPitchMax", 70.0F},
    {"rotationChangeYaw", false},
    {"Tower", false},
    {"TowerMotionY", 0.5F},
    {"TowerPitchMin", 85.0F},
    {"TowerPitchMax", 95.0F},
    {"debug", false},
    {"Extend", 0},
};
static bool InTower = false;
static float YCoord{};
static MemTool::Hook Helper_Block_tryScaffold;
static MemTool::Hook Helper_Block_tryClutchScaffold;
glm::vec3 targetBlock{};
static bool Helper_Block_tryScaffold_(LocalPlayer *player, glm::vec3 blockBelow, bool strict) {
  auto ret = Helper_Block_tryScaffold.call<bool>(player, blockBelow);
  try {
    if (g_md->getGUI().Get<bool>("debug")) {
      g_log_tool.message(LogLevel::DEBUG, "Scaffold",
                         "tryScaffold : {} {} {} status: {} strict: {}", blockBelow.x, blockBelow.y,
                         blockBelow.z, ret, strict);
    }
  } catch (...) {
  }
  targetBlock = blockBelow;
  return ret;
}
static bool Helper_Block_tryClutchScaffold_(LocalPlayer *player, BlockSource *region,
                                            glm::vec3 blockBelow, bool strict) {
  auto ret = Helper_Block_tryClutchScaffold.call<bool>(player, region, blockBelow);
  try {
    if (g_md->getGUI().Get<bool>("debug")) {
      g_log_tool.message(LogLevel::DEBUG, "Scaffold",
                         "tryClutchScaffold : {} {} {} status: {} strict: {}", blockBelow.x,
                         blockBelow.y, blockBelow.z, ret, strict);
    }
  } catch (...) {
  }
  targetBlock = blockBelow;
  return ret;
}
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::WORLD_MENU, ConfigData) {
  setOnLoad([](Module *module) {
    g_md = module;
    Helper_Block_tryScaffold =
        MemTool::Hook(&Helper::Block::tryScaffold,
                      reinterpret_cast<void *>(Helper_Block_tryScaffold_), nullptr, false);
    Helper_Block_tryClutchScaffold =
        MemTool::Hook(&Helper::Block::tryClutchScaffold,
                      reinterpret_cast<void *>(Helper_Block_tryClutchScaffold_), nullptr, false);
  });
  setOnEnable([](Module *module) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;
      glm::vec3 blockBelow = Helper::Block::getBlockBelow(player, 0.5);
      YCoord = blockBelow.y;
    } catch (...) {
    }
  });
  setOnDisable([](Module *module) { InTower = false; });
  setOnDrawGUI([](Module *module) {
    module->getGUI().CheckBox("DownMode", "楼梯模式");
    module->getGUI().CheckBox("placeStrict", "严格放置");
    module->getGUI().CheckBox("SameY", "同高度放置");
    module->getGUI().SliderInt("Extend", "扩展", 0, 10);
    if (ImGui::TreeNode("Tower")) {
      module->getGUI().CheckBox("Tower", "塔模式");
      module->getGUI().SliderFloat("TowerMotionY", "塔模式高度", 0.0F, 1.0F);
      module->getGUI().SliderFloat("TowerPitchMin", "塔模式最小角度", 0.0F, 180.0F);
      module->getGUI().SliderFloat("TowerPitchMax", "塔模式最大角度", 0.0F, 180.0F);

      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Rotation")) {
      module->getGUI().CheckBox("rotation", "转头");
      module->getGUI().CheckBox("rotationChangeYaw", "转头改变Yaw");
      module->getGUI().SliderFloat("rotationPitchMin", "转头最小角度", 0.0F, 180.0F);
      module->getGUI().SliderFloat("rotationPitchMax", "转头最大角度", 0.0F, 180.0F);
      module->getGUI().CheckBox("rotationSlient", "静音转头");
      ImGui::TreePop();
    }
    module->getGUI().CheckBox("debug", "调试模式");
  });
  setOnRender([](Module *module) {
    try {
      std::random_device rd;
      std::mt19937 gen(rd());
      bool DownMode = module->getGUI().Get<bool>("DownMode");
      bool rotation = module->getGUI().Get<bool>("rotation");
      bool rotationSlient = module->getGUI().Get<bool>("rotationSlient");
      bool Tower = module->getGUI().Get<bool>("Tower");
      float TowerMotionY = module->getGUI().Get<float>("TowerMotionY");
      bool rotationChangeYaw = module->getGUI().Get<bool>("rotationChangeYaw");
      bool placeStrict = module->getGUI().Get<bool>("placeStrict");
      bool SameY = module->getGUI().Get<bool>("SameY");
      int Extend = module->getGUI().Get<int>("Extend");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;

      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;
      BlockSource *region = instance->getRegion();
      if (!region)
        return;
      ItemStack *item = player->getSelectedItem();
      if (!item || !item->isBlock())
        return;
      Helper::Rotation::Rotation rot =
          Helper::Rotation::toRotation(player->getPosition(), targetBlock);
      float yaw = rotationChangeYaw ? rot.yaw : player->getYaw();
      if (rotation && !rotationSlient) {
        if (InTower) {
          float TowerPitchMin = module->getGUI().Get<float>("TowerPitchMin");
          float TowerPitchMax = module->getGUI().Get<float>("TowerPitchMax");
          std::uniform_real_distribution<> distrib(TowerPitchMin, TowerPitchMax);
          float randomPitch = distrib(gen);

          player->setPitch(randomPitch);
          player->setYaw(yaw);
        } else {
          float rotationPitchMin = module->getGUI().Get<float>("rotationPitchMin");
          float rotationPitchMax = module->getGUI().Get<float>("rotationPitchMax");
          std::uniform_real_distribution<> distrib(rotationPitchMin, rotationPitchMax);
          float randomPitch = distrib(gen);
          player->setPitch(randomPitch);
          player->setYaw(yaw);
        }
      }
      glm::vec3 motion = player->getMotion();
      float speed = glm::length(glm::vec2(motion.x, motion.y));
      if (player->isJumping() && Tower && !InTower) {
        InTower = true;
        player->setMotion(glm::vec3(motion.x, TowerMotionY, motion.z));
      } else if (InTower && ActorCollision::isOnGround(player->mEntityContext)) {
        InTower = false;
      }
      glm::vec3 vel = glm::normalize(motion);
      if (DownMode) {
        glm::vec3 blockBelow = Helper::Block::getBlockBelow(player, 1.5f);
        glm::vec3 blockBelowBelow = Helper::Block::getBlockBelow(player, 2.0f);
        if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict) &&
            !Helper::Block::tryScaffold(player, blockBelowBelow, placeStrict)) {
          if (speed > 0.05f) {
            blockBelow.z -= vel.z * 0.4f;
            blockBelowBelow.z -= vel.z * 0.4f;
            if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict) &&
                !Helper::Block::tryScaffold(player, blockBelowBelow, placeStrict)) {
              blockBelow.x -= vel.x * 0.4f;
              blockBelowBelow.x -= vel.x * 0.4f;
              if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict) &&
                  !Helper::Block::tryScaffold(player, blockBelowBelow, placeStrict) &&
                  player->isSprinting()) {
                blockBelow.z += vel.z;
                blockBelow.x += vel.x;
                blockBelowBelow.z += vel.z;
                blockBelowBelow.x += vel.x;
                Helper::Block::tryScaffold(player, blockBelow, placeStrict);
                Helper::Block::tryScaffold(player, blockBelowBelow, placeStrict);
              }
            }
          }
        }
      } else {
        glm::vec3 blockBelowReal = Helper::Block::getBlockBelow(player, 1.5f);
        glm::vec3 blockBelow = blockBelowReal;
        if (SameY) {
          Helper::Block::adjustYCoordinate(blockBelow, blockBelowReal, YCoord);
        }
        Helper::Block::extendBlock(vel, blockBelow, Extend);

        if (Helper::Block::isAirBlock(blockBelow)) {
          Helper::Block::tryClutchScaffold(player, region, blockBelow, placeStrict);
        } else {
          if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict)) {
            if (speed > 0.05f) {
              blockBelow.z -= vel.z * 0.4f;

              if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict)) {
                blockBelow.x -= vel.x * 0.4f;

                if (!Helper::Block::tryScaffold(player, blockBelow, placeStrict) &&
                    player->isSprinting()) {
                  blockBelow.z += vel.z;
                  blockBelow.x += vel.x;

                  Helper::Block::tryScaffold(player, blockBelow, placeStrict);
                }
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
      bool rotationChangeYaw = g_md->getGUI().Get<bool>("rotationChangeYaw");

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
      float pitch = InTower ? g_md->getGUI().Get<float>("TowerPitchMax") : g_md->getGUI().Get<float>("rotationPitchMax");
      Helper::Rotation::Rotation rot = Helper::Rotation::toRotation(pos, targetBlock);
      float yaw = rotationChangeYaw ? rot.yaw : player->getYaw();
      if (packet->getName() == "MovePlayerPacket") {
        auto *movePacket = static_cast<MovePlayerPacket *>(packet);
        movePacket->mRot = glm::vec2(pitch, yaw);
      } else if (packet->getName() == "PlayerAuthInputPacket") {
        auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
        authPacket->mRot = glm::vec2(pitch, yaw);
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}
