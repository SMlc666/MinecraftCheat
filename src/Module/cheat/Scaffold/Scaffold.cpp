#include "Scaffold.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "game/minecraft/world/level/block/Block.hpp"
#include "game/minecraft/world/level/block/BlockLegacy.hpp"
#include "game/minecraft/world/level/block/BlockPos.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"swing", false}};
static bool canPlace(const glm::vec3 &pos) {
  Block *block = runtimes::getClientInstance()->getRegion()->getBlock(
      static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
  if ((block == nullptr) || (block->mBlockLegacy == nullptr)) {
    return false;
  }
  return block->mBlockLegacy->getName().find("air") != std::string::npos;
}
static void buildScaffold(LocalPlayer *player, const BlockPos &pos, uchar face) {
  if (auto *gameMode = &player->getGameMode(); gameMode != nullptr) {
    gameMode->buildBlock(pos, face);
    if (g_md->getGUI().Get<bool>("swing")) {
      player->swing();
    }
  }
}

cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) { module->getGUI().CheckBox("swing", "挥手"); });
  setOnRender([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (instance == nullptr) {
      return;
    }
    auto *player = instance->getLocalPlayer();
    if (player == nullptr) {
      return;
    }
    ItemStack *item = player->getSelectedItem();
    if (item == nullptr || !item->isBlock()) {
      return;
    }

    glm::vec3 playerPos = player->getPosition();
    glm::vec3 motion = player->getMotion();

    // 获取玩家视角
    float yaw = player->getYaw();
    // 将角度转换为弧度
    float yawRad = yaw * (M_PI / 180.0f);

    // 计算主要朝向 (-180到180度分为8个区域)
    int direction = static_cast<int>((yaw + 180.0f + 22.5f) / 45.0f) % 8;

    // 可能的放置位置
    std::vector<glm::vec3> possiblePositions;
    glm::vec3 basePos = playerPos;
    basePos.y -= 1.0f; // 脚下位置

    // 根据朝向添加主要方向和相邻方向的方块位置
    switch (direction) {
    case 0:                                                                   // 北 (Z-)
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z - 1});     // 前
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z - 1}); // 左前
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z - 1}); // 右前
      break;

    case 1:                                                                   // 东北
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z - 1}); // 右前
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z});     // 右
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z - 1});     // 前
      break;

    case 2:                                                                   // 东 (X+)
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z});     // 右
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z - 1}); // 右前
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z + 1}); // 右后
      break;

    case 3:                                                                   // 东南
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z + 1}); // 右后
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z});     // 右
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z + 1});     // 后
      break;

    case 4:                                                                   // 南 (Z+)
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z + 1});     // 后
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z + 1}); // 左后
      possiblePositions.push_back({basePos.x + 1, basePos.y, basePos.z + 1}); // 右后
      break;

    case 5:                                                                   // 西南
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z + 1}); // 左后
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z});     // 左
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z + 1});     // 后
      break;

    case 6:                                                                   // 西 (X-)
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z});     // 左
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z - 1}); // 左前
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z + 1}); // 左后
      break;

    case 7:                                                                   // 西北
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z - 1}); // 左前
      possiblePositions.push_back({basePos.x - 1, basePos.y, basePos.z});     // 左
      possiblePositions.push_back({basePos.x, basePos.y, basePos.z - 1});     // 前
      break;
    }

    // 添加脚下的方块（当玩家上升时）
    if (motion.y > 0) {
      possiblePositions.push_back(basePos);
    }

    // 遍历所有可能的位置并尝试放置方块
    for (const auto &pos : possiblePositions) {
      if (canPlace(pos)) {
        BlockPos placePos(static_cast<int>(pos.x), static_cast<int>(pos.y),
                          static_cast<int>(pos.z));

        // 计算放置面
        uchar face = 1; // 默认向上
        glm::vec3 diff = playerPos - pos;
        if (std::abs(diff.y) > std::abs(diff.x) && std::abs(diff.y) > std::abs(diff.z)) {
          face = (diff.y > 0) ? 0 : 1;
        } else if (std::abs(diff.x) > std::abs(diff.z)) {
          face = (diff.x > 0) ? 4 : 5;
        } else {
          face = (diff.z > 0) ? 2 : 3;
        }

        buildScaffold(player, placePos, face);
      }
    }
  });
}
