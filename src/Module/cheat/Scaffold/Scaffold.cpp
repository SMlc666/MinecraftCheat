#include "Scaffold.hpp"
#include "Helper/Block/Block.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/actor/provider/ActorCollision.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/item/ItemStack.hpp"
#include "glm/fwd.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
#include <vector>
static Module *g_md{};
const static std::unordered_map<std::string, std::any> ConfigData = {{"enabled", false},
                                                                     {"shortcut", false}};
static float targetY = 0.5F;
cheat::Scaffold::Scaffold() : Module("Scaffold", MenuType::COMBAT_MENU, ConfigData) {
  setOnLoad([](Module *module) { g_md = module; });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnRender([](Module *module) {
    ClientInstance *instance = runtimes::getClientInstance();
    if (!instance)
      return;

    LocalPlayer *player = instance->getLocalPlayer();
    if (!player || player->getMotion().y > 0)
      return; // 只在非上升状态工作

    ItemStack *item = player->getSelectedItem();
    if (!item || !item->isBlock())
      return;

    // 获取玩家位置和方向
    glm::vec3 playerPos = player->getPosition();
    float yaw = player->getYaw();

    // 定义6个面方向（下、上、北、南、西、东）
    static const std::vector<glm::ivec3> neighbours = {{0, -1, 0}, {0, 1, 0},  {0, 0, -1},
                                                       {0, 0, 1},  {-1, 0, 0}, {1, 0, 0}};

    /* 自动补脚下方块 */
    glm::ivec3 feetBlock(glm::floor(playerPos.x),
                         glm::floor(playerPos.y - 1.0f), // 稍微向下取整
                         glm::floor(playerPos.z));

    if (Helper::Block::isAirBlock(feetBlock)) {
      for (int face = 0; face < neighbours.size(); ++face) {
        glm::ivec3 supportBlock = feetBlock - neighbours[face];
        if (!Helper::Block::isAirBlock(supportBlock)) {
          player->getGameMode().buildBlock(supportBlock, static_cast<unsigned char>(face));
          break;
        }
      }
    }

    /* 自动向前延伸 */
    // 计算水平方向向量
    glm::vec3 frontDir(-glm::sin(glm::radians(yaw)), 0.0f, glm::cos(glm::radians(yaw)));
    frontDir = glm::normalize(frontDir);

    // 计算前方2.5格位置（浮点坐标）
    glm::vec3 targetPos = playerPos + frontDir * 2.5f;

    // 转换为方块坐标并获取下方方块
    glm::ivec3 targetBlock(glm::floor(targetPos.x), glm::floor(targetPos.y),
                           glm::floor(targetPos.z));
    glm::ivec3 belowTarget = targetBlock - glm::ivec3(0, 1, 0);

    // 检查是否需要放置支撑
    if (Helper::Block::isAirBlock(belowTarget)) {
      for (int face = 0; face < neighbours.size(); ++face) {
        glm::ivec3 supportBlock = belowTarget - neighbours[face];
        if (!Helper::Block::isAirBlock(supportBlock)) {
          player->getGameMode().buildBlock(supportBlock, static_cast<unsigned char>(face));
          break;
        }
      }
    }
  });
}
