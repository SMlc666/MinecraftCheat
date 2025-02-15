#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include "log.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>

static Module *g_md{};
static glm::vec3 targetBlock{};
static MemTool::Hook Helper_Block_tryScaffold;

static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"placeStrict", false},
    {"AirPlace", true},
    {"debug", false},
    {"Extend", 0.2f} // 新增扩展距离参数
};

static bool Helper_Block_tryScaffold_(LocalPlayer *player, glm::vec3 blockBelow, bool strict) {
    auto ret = Helper_Block_tryScaffold.call<bool>(player, blockBelow);
    if (g_md->getGUI().Get<bool>("debug")) {
        g_log_tool.message(LogLevel::DEBUG, "Scaffold", "tryScaffold at ({}, {}, {}) -> {}", blockBelow.x, blockBelow.y, blockBelow.z, ret);
    }
    targetBlock = blockBelow;
    return ret;
}

cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::WORLD_MENU, ConfigData) {
    setOnLoad([](Module *module) {
        g_md = module;
        Helper_Block_tryScaffold = MemTool::Hook(&Helper::Block::tryScaffold, reinterpret_cast<void *>(Helper_Block_tryScaffold_), nullptr, false);
    });

    setOnDrawGUI([](Module *module) {
        module->getGUI().CheckBox("AirPlace", "空气放置");
        module->getGUI().CheckBox("placeStrict", "严格放置");
        module->getGUI().SliderFloat("Extend", "延伸距离", 0.2f, 3.0f); // 新增滑动条控制延伸距离
        module->getGUI().CheckBox("debug", "调试模式");
    });

    setOnRender([](Module *module) {
        ClientInstance *instance = runtimes::getClientInstance();
        if (!instance) return;
        LocalPlayer *player = instance->getLocalPlayer();
        if (!player) return;
        BlockSource *region = instance->getRegion();
        if (!region) return;

        bool AirPlace = module->getGUI().Get<bool>("AirPlace");
        bool placeStrict = module->getGUI().Get<bool>("placeStrict");
        float extendDistance = module->getGUI().Get<float>("Extend"); // 获取延伸距离

        // 处理脚下位置
        glm::vec3 originalBlockBelow = Helper::Block::getBlockBelow(player, 1.5f);
        if (AirPlace) originalBlockBelow.y -= 1.0f;
        Helper_Block_tryScaffold_(player, originalBlockBelow, placeStrict);

        // 获取玩家位置和视角方向
        glm::vec3 playerPos = player->getPosition();
        float yaw = player->getYaw();
        float yawRad = glm::radians(yaw);

        // 计算移动方向
        glm::vec3 moveDir = glm::vec3(sin(yawRad), 0.0f, cos(yawRad));

        // 计算前方延伸位置
        glm::vec3 frontPos = playerPos + moveDir * extendDistance;
        glm::vec3 frontBlockBelow = frontPos;
        frontBlockBelow.y -= 1.5f; // 假设 getBlockBelow 只需要玩家和 Y 偏移
        if (AirPlace) frontBlockBelow.y -= 1.0f;
        Helper_Block_tryScaffold_(player, frontBlockBelow, placeStrict);

      if (player->isJumping()) { // 如果玩家在跳跃
         glm::vec3 jumpBlockBelow = playerPos;
         jumpBlockBelow.y -= 0.5f; // 更接近脚部
      if (AirPlace) jumpBlockBelow.y -= 1.0f;
         Helper_Block_tryScaffold_(player, jumpBlockBelow, true); // 严格模式确保放置
        }
    });
}