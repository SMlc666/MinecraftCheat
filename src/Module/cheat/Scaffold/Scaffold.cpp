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

    // 计算脚底基准坐标
    glm::ivec3 basePos(static_cast<int>(std::floor(pos.x)), static_cast<int>(std::floor(pos.y)) - 1,
                       static_cast<int>(std::floor(pos.z)));

    // 筛选周围8个候选方块
    std::vector<glm::ivec3> candidates;
    for (auto &blockPos : canBuildBlocks) {
      if (blockPos.y == basePos.y && std::abs(blockPos.x - basePos.x) <= 1 &&
          std::abs(blockPos.z - basePos.z) <= 1 &&
          !(blockPos.x == basePos.x && blockPos.z == basePos.z)) {
        candidates.push_back(blockPos);
      }
    }

    if (candidates.empty())
      return;

    // 计算方向向量
    float yawRad = glm::radians(player->getYaw());
    glm::vec2 frontDir(-std::sin(yawRad), std::cos(yawRad));

    // 优先级排序（按朝向相似度）
    std::sort(candidates.begin(), candidates.end(), [&](const glm::ivec3 &a, const glm::ivec3 &b) {
      glm::vec2 aOffset(a.x - basePos.x, a.z - basePos.z);
      glm::vec2 bOffset(b.x - basePos.x, b.z - basePos.z);
      return glm::dot(aOffset, frontDir) > glm::dot(bOffset, frontDir);
    });

    // 遍历候选位置放置方块
    for (auto &targetPos : candidates) {
      // 计算相对偏移
      glm::ivec3 offset = targetPos - basePos;
      uchar face = 0;

      // 判断主要偏移方向
      if (offset.x != 0) {
        face = offset.x > 0 ? 5 /*EAST*/ : 4 /*WEST*/;
      }
      if (offset.z != 0) {
        // 当同时存在x偏移时，根据绝对值大小决定主方向
        if (offset.x == 0 || std::abs(offset.z) > std::abs(offset.x)) {
          face = offset.z > 0 ? 3 /*SOUTH*/ : 2 /*NORTH*/;
        }
      }

      // 执行方块建造
      Helper::Block::buildBlock(player, targetPos, face, true);
      break; // 优先放置最符合方向的一个方块
    }
  });
}