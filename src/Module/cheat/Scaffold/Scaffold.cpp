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
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"swing", false}};
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().CheckBox("swing", "挥手"); });
  setOnRender([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr)
      return;

    LocalPlayer *player = instance->getLocalPlayer();
    if (!player || player->getMotion().y <= 0)
      return;

    ItemStack *item = player->getSelectedItem();
    if (!item || !item->isBlock())
      return;

    glm::vec3 pos = player->getPosition();
    auto canBuildBlocks = Helper::Block::getCanBuildBlocks(player, pos);
    if (canBuildBlocks.empty())
      return;

  // 在框架的onRender回调末尾添加以下代码：
    glm::ivec3 basePos(
        static_cast<int>(std::floor(pos.x)),
        static_cast<int>(std::floor(pos.y)) - 1,
        static_cast<int>(std::floor(pos.z))
    );

    // 优先检查脚下方块
    if (Helper::Block::canPlaceBlock(basePos)) {
        static const std::vector<glm::ivec3> neighbourOffsets = {
            {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, 
            {0, 0, 1}, {-1, 0, 0}, {1, 0, 0}
        };

        for (int face = 0; face < 6; ++face) {
            glm::ivec3 neighbour = basePos + neighbourOffsets[face];
            if (!Helper::Block::isAirBlock(neighbour)) {
                bool swing = module->getConfigValue<bool>("swing");
                Helper::Block::buildBlock(player, basePos, face, swing);
                return;
            }
        }
    }

    // 生成周围8个候选位置
    std::vector<glm::ivec3> candidates;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dz = -1; dz <= 1; ++dz) {
            if (dx == 0 && dz == 0) continue;
            candidates.emplace_back(basePos.x + dx, basePos.y, basePos.z + dz);
        }
    }

    // 计算方向向量
    const float yawRad = glm::radians(yaw);
    const glm::vec3 forwardDir(-sin(yawRad), 0.0f, cos(yawRad));

    // 优先级排序（使用带权重的评分机制）
    std::vector<std::pair<glm::ivec3, float>> scoredPositions;
    for (const auto& pos : candidates) {
        const glm::vec3 offset(pos.x - basePos.x, 0, pos.z - basePos.z);
        const glm::vec3 dir = glm::normalize(offset);
        
        // 方向权重（前向优先）
        const float forwardWeight = glm::dot(dir, forwardDir);
        // 距离权重（近处优先）
        const float distanceWeight = 1.0f / (glm::length(offset) + 0.1f);
        
        scoredPositions.emplace_back(pos, forwardWeight + distanceWeight);
    }

    // 按综合评分降序排序
    std::sort(scoredPositions.begin(), scoredPositions.end(), 
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // 尝试放置方块
    for (const auto& [targetPos, score] : scoredPositions) {
        if (!Helper::Block::canPlaceBlock(targetPos)) continue;

        // 寻找可依附的面
        static const std::vector<std::pair<glm::ivec3, uchar>> faceChecks = {
            {{0, -1, 0}, 1},   // 需要下方有方块（向上放置）
            {{0, 0, -1}, 3},   // 需要北面有方块（向南放置）
            {{0, 0, 1}, 2},    // 需要南面有方块（向北放置）
            {{-1, 0, 0}, 5},   // 需要西面有方块（向东放置）
            {{1, 0, 0}, 4},    // 需要东面有方块（向西放置）
            {{0, 1, 0}, 0}     // 需要上方有方块（向下放置）
        };

        for (const auto& [offset, face] : faceChecks) {
            const glm::ivec3 checkPos = targetPos + offset;
            if (!Helper::Block::isAirBlock(checkPos)) {
                bool swing = module->getConfigValue<bool>("swing");
                Helper::Block::buildBlock(player, targetPos, face, swing);
                return; // 成功放置后退出
            }
        }
    }
  });
}
