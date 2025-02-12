#include "KillAura.hpp"
#include <cmath>
#include "Helper/Target/Target.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/minecraft.hpp"
#include "game/minecraft/network/LoopbackPacketSender.hpp"
#include "game/minecraft/network/Packet/Packets/AnimatePacket.hpp"
#include "game/minecraft/network/Packet/Packets/MovePlayerPacket.hpp"
#include "game/minecraft/network/Packet/Packets/PlayerAuthInputPacket.hpp"
#include "game/minecraft/network/PacketSender.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <chrono>
#include <string>
#include <unordered_map>
#include <random>
#include "Helper/Rotation/rotation.hpp"
static const std::vector<std::string> PriorityItems = {"Health", "Distance", "Random"};
static const std::vector<std::string> RotationItems = {"Lock", "Approximate"};
static const std::vector<std::string> SwingItems = {"Client", "Server", "Both"};
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"mincps", 10},
    {"maxcps", 20},
    {"range", 5.0F},
    {"swing", 2},
    {"attackNum", 1},
    {"antibot", false},
    {"fov", 180.0F},
    {"failurerate", 0},
    {"priority", 0},
    {"rotation", false},
    {"rotationSmooth", false},
    {"rotationSmoothFactor", 0.1F},
    {"rotationSmoothMinStep", 0.01F},
    {"rotationSmoothMaxStep", 1.0F},
    {"rotationSmoothDeltaTime", 0.016F},
    {"rotationMode", 0},
    {"rotationSlient", false}};
static std::vector<Player *> PlayerList = {};
static std::chrono::steady_clock::time_point LastAttackTime = std::chrono::steady_clock::now();
static std::random_device g_rd;
static std::uniform_int_distribution<> g_dist(0, 100);
static std::mt19937 g_gen(g_rd());
static Player *g_Target{};
static Helper::Rotation::Rotation last = {0.0F, 0.0F};
cheat::KillAura::KillAura() : Module("KillAura", MenuType::COMBAT_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) { g_Target = nullptr; });
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.SliderFloat("range", "范围", 0.0F, 10.0F);
    gui.SliderInt("mincps", "最小攻击速度", 1, 20, [module](int value) {
      int maxcps = module->getGUI().Get<int>("maxcps");
      if (value > maxcps) {
        module->getGUI().Set<int>("maxcps", value);
      }
    });
    gui.SliderInt("maxcps", "最大攻击速度", 1, 20);
    gui.SliderInt("attackNum", "攻击数量", 1, 20);
    gui.Selectable("swing", "挥手", SwingItems);
    gui.CheckBox("antibot", "反机器人");
    gui.SliderFloat("fov", "视角", 0.0F, 360.0F);
    gui.SliderInt("failurerate", "失败率", 0, 100);
    gui.Selectable("priority", "优先级", PriorityItems);
    if (ImGui::TreeNode("Rotation")) {
      gui.CheckBox("rotation", "转头");
      gui.Selectable("rotationMode", "转头模式", RotationItems);
      gui.CheckBox("rotationSlient", "无声转头");
      if (ImGui::TreeNode("Rotation Smooth")) {
        gui.CheckBox("rotationSmooth", "平滑转头");
        gui.SliderFloat("rotationSmoothFactor", "平滑系数", 0.01f, 1.0F);
        gui.SliderFloat("rotationSmoothMinStep", "最小步长", 0.01f, 1.0F, [&module](float value) {
          try {
            float rotationSmoothMaxStep = module->getGUI().Get<float>("rotationSmoothMaxStep");
            if (value > rotationSmoothMaxStep) {
              module->getGUI().Set<float>("rotationSmoothMaxStep", value);
            }
          } catch (...) {
            return;
          }
        });
        gui.SliderFloat("rotationSmoothMaxStep", "最大步长", 0.01f, 1.0F);
        gui.SliderFloat("rotationSmoothDeltaTime", "平滑时间", 0.001f, 1.0F);
        ImGui::TreePop();
      }
      ImGui::TreePop();
    }
  });
  setOnTick([](Module *module) {
    float Range = NAN;
    int swing{};
    int mincps = 0;
    int maxcps = 0;
    int attackNum = 0;
    bool antibot = false;
    int failurerate = 0;
    float fov = NAN;
    int priority = 0;
    try {
      Range = module->getGUI().Get<float>("range");
      swing = module->getGUI().Get<int>("swing");
      mincps = module->getGUI().Get<int>("mincps");
      maxcps = module->getGUI().Get<int>("maxcps");
      attackNum = module->getGUI().Get<int>("attackNum");
      antibot = module->getGUI().Get<bool>("antibot");
      failurerate = module->getGUI().Get<int>("failurerate");
      fov = module->getGUI().Get<float>("fov");
      priority = module->getGUI().Get<int>("priority");
    } catch (const std::exception &e) {
      g_log_tool.message(LogLevel::ERROR, "KillAura", std::format("读取配置失败:{}", e.what()));
      return;
    }
    int attackCount = 0;
    auto now = std::chrono::steady_clock::now();
    int rmcps = std::uniform_int_distribution<>(std::min(mincps, maxcps), maxcps)(g_gen);
    auto interval = std::chrono::milliseconds(1000 / rmcps);
    if (now - LastAttackTime < interval) {
      return;
    }
    ClientInstance *mInstance = runtimes::getClientInstance();
    if (mInstance == nullptr) {
      return;
    }
    LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
    if (mLocalPlayer == nullptr) {
      return;
    }
    Dimension *mDimension = mLocalPlayer->mDimension;
    if (mDimension == nullptr) {
      return;
    }
    Minecraft *mMinecraft = mInstance->minecraftPtr;
    if (mMinecraft == nullptr) {
      return;
    }
    LoopbackPacketSender *mPacketSender = runtimes::getPacketSender();
    if (mPacketSender == nullptr) {
      return;
    }
    PlayerList.clear();
    mDimension->forEachPlayer([=](Player &player) {
      return Helper::Target::ProcessPlayer(player, mLocalPlayer, antibot, Range, fov, PlayerList);
    });
    if (!PlayerList.empty()) {
      g_Target = nullptr;
      if (priority == 0) {
        std::sort(PlayerList.begin(), PlayerList.end(),
                  [](Player *a, Player *b) { return a->getHealth() < b->getHealth(); });
      } else if (priority == 1) {
        std::sort(PlayerList.begin(), PlayerList.end(), [mLocalPlayer](Player *a, Player *b) {
          return mLocalPlayer->getDistance(a) < mLocalPlayer->getDistance(b);
        });
      } else if (priority == 2) {
        std::shuffle(PlayerList.begin(), PlayerList.end(), g_gen);
      }
    } else {
      return;
    }
    GameMode *mGameMode = &mLocalPlayer->getGameMode(); //LocalPlayer
    bool hasAttacked = false;
    for (auto *player : PlayerList) {
      if (attackCount >= attackNum) {
        break;
      }
      attackCount++;
      if (failurerate <= 0 || g_dist(g_gen) >= failurerate) {
        mGameMode->attack(*player);
        g_Target = player;
        hasAttacked = true;
      }
      if (swing == 0 || swing == 2) {
        mLocalPlayer->swing();
      }
      if (swing == 1 || swing == 2) {
        AnimatePacket packet(AnimatePacket::Action::Swing, *mLocalPlayer);
        mPacketSender->sendToServer(reinterpret_cast<Packet &>(packet));
      }
    }
    if (hasAttacked) {
      LastAttackTime = std::chrono::steady_clock::now();
    }
  });
  setOnRender([](Module *module) {
    using namespace Helper;
    bool rotation = false;
    bool antibot = false;
    bool rotationSmooth = false;
    float rotationSmoothFactor;
    float rotationSmoothMinStep;
    float rotationSmoothMaxStep;
    float rotationSmoothDeltaTime;

    float fov = NAN;
    float Range = NAN;
    int rotationMode = 0;
    bool rotationSlient = false;
    try {
      rotation = module->getGUI().Get<bool>("rotation");
      rotationMode = module->getGUI().Get<int>("rotationMode");
      antibot = module->getGUI().Get<bool>("antibot");
      fov = module->getGUI().Get<float>("fov");
      Range = module->getGUI().Get<float>("range");
      rotationSlient = module->getGUI().Get<bool>("rotationSlient");
      rotationSmooth = module->getGUI().Get<bool>("rotationSmooth");
      rotationSmoothFactor = module->getGUI().Get<float>("rotationSmoothFactor");
      rotationSmoothMinStep = module->getGUI().Get<float>("rotationSmoothMinStep");
      rotationSmoothMaxStep = module->getGUI().Get<float>("rotationSmoothMaxStep");
      rotationSmoothDeltaTime = module->getGUI().Get<float>("rotationSmoothDeltaTime");
    } catch (const std::exception &e) {
      return;
    }
    if (rotationSlient && rotation) {
      return;
    }
    if (g_Target == nullptr) {
      return;
    }
    ClientInstance *mInstance = runtimes::getClientInstance();
    if (mInstance == nullptr) {
      return;
    }
    LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
    if (mLocalPlayer == nullptr) {
      return;
    }
    if (!Helper::Target::hasPlayer(mLocalPlayer->mDimension, g_Target, mLocalPlayer, antibot, Range,
                                   fov)) {
      return;
    }
    glm::vec3 localPos = mLocalPlayer->getPosition();
    glm::vec3 targetPos = g_Target->getPosition();
    Rotation::Rotation aimTarget = Rotation::toRotation(localPos, targetPos);
    last = {mLocalPlayer->getPitch(), mLocalPlayer->getYaw()};
    if (rotationSmooth) {
      aimTarget = Rotation::interpolateRotation(last, aimTarget, rotationSmoothDeltaTime,
                                                rotationSmoothMinStep, rotationSmoothMaxStep,
                                                rotationSmoothFactor);
    }
    switch (rotationMode) {
    case 0:
      mLocalPlayer->setPitch(aimTarget.pitch);
      mLocalPlayer->setYaw(aimTarget.yaw);
      break;
    case 1:
      aimTarget.pitch /= 2.0F;
      float diff = Rotation::getRotationDifference(last, aimTarget);
      if (diff >= 50.0F) {
        mLocalPlayer->setPitch((aimTarget.pitch - last.pitch) / 0.8F + last.pitch);
        mLocalPlayer->setYaw((aimTarget.yaw - last.yaw) / 0.8F + last.yaw);
      }
      break;
    }
  });
  setOnSendPacket([](Module *module, Packet *packet) {
    if (packet->getName() == "PlayerAuthInputPacket") {
      auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
      last.pitch = authPacket->mRot->x;
      last.yaw = authPacket->mRot->y;
    } else if (packet->getName() == "MovePlayerPacket") {
      auto *movePacket = static_cast<MovePlayerPacket *>(packet);
      last.pitch = movePacket->mRot.x;
      last.yaw = movePacket->mRot.y;
    } else {
      return true;
    }
    try {
      if (g_Target == nullptr) {
        return true;
      }
      if (packet->getName() != "MovePlayerPacket" && packet->getName() != "PlayerAuthInputPacket") {
        return true;
      }
      auto &gui = module->getGUI();
      bool rotation = gui.Get<bool>("rotation");
      bool rotationSlient = gui.Get<bool>("rotationSlient");
      int rotationMode = gui.Get<int>("rotationMode");
      bool antibot = gui.Get<bool>("antibot");
      float fov = gui.Get<float>("fov");
      float Range = gui.Get<float>("range");
      if (!rotation || !rotationSlient) {
        return true;
      }
      ClientInstance *mInstance = runtimes::getClientInstance();
      if (mInstance == nullptr) {
        return true;
      }
      LocalPlayer *mLocalPlayer = mInstance->getLocalPlayer();
      if (mLocalPlayer == nullptr) {
        return true;
      }
      if (!Helper::Target::hasPlayer(mLocalPlayer->mDimension, g_Target, mLocalPlayer, antibot,
                                     Range, fov)) {
        return true;
      }
      glm::vec3 localPos = mLocalPlayer->getPosition();
      glm::vec3 targetPos = g_Target->getPosition();
      Helper::Rotation::Rotation aimTarget = Helper::Rotation::toRotation(localPos, targetPos);
      switch (rotationMode) {
      case 0:
        if (packet->getName() == "MovePlayerPacket") {
          auto *movePacket = static_cast<MovePlayerPacket *>(packet);
          movePacket->mYHeadRot = aimTarget.yaw;
          movePacket->mRot = glm::vec2{aimTarget.pitch, aimTarget.yaw};
        } else {
          auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
          authPacket->mYHeadRot = aimTarget.yaw;
          authPacket->mRot = glm::vec2{aimTarget.pitch, aimTarget.yaw};
        }
        break;
      case 1:
        aimTarget.pitch /= 2.0F;
        float diff = Helper::Rotation::getRotationDifference(last, aimTarget);
        if (diff >= 50.0F) {
          if (packet->getName() == "MovePlayerPacket") {
            auto *movePacket = static_cast<MovePlayerPacket *>(packet);
            movePacket->mYHeadRot = (aimTarget.yaw - last.yaw) / 0.8F + last.yaw;
            movePacket->mRot = glm::vec2((aimTarget.pitch - last.pitch) / 0.8F + last.pitch,
                                         (aimTarget.yaw - last.yaw) / 0.8F + last.yaw);
          } else {
            auto *authPacket = static_cast<PlayerAuthInputPacket *>(packet);
            authPacket->mYHeadRot = (aimTarget.yaw - last.yaw) / 0.8F + last.yaw;
            authPacket->mRot = glm::vec2((aimTarget.pitch - last.pitch) / 0.8F + last.pitch,
                                         (aimTarget.yaw - last.yaw) / 0.8F + last.yaw);
          }
        }
        break;
      }
    } catch (...) {
      return true;
    }
    return true;
  });
}