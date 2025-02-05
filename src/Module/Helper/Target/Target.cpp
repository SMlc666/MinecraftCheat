#include "Target.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
bool Helper::Target::hasPlayer(Dimension *dimension, Player *targetPlayer, LocalPlayer *localPlayer,
                               bool antibot, float range, float fov) {
  bool foundPlayer = false;
  std::vector<Player *> playerList;
  dimension->forEachPlayer([&](Player &player) {
    return ProcessPlayer(player, localPlayer, antibot, range, fov, playerList);
  });
  for (Player *player : playerList) {
    if (player == targetPlayer) {
      foundPlayer = true;
      break;
    }
  }
  return foundPlayer;
}
bool Helper::Target::isBot(Player *player) {
  float Pitch = player->getPitch();
  float Yaw = player->getYaw();
  return Yaw == 0.0F && Pitch == 0.0F;
}
bool Helper::Target::ProcessPlayer(Player &player, LocalPlayer *localPlayer, bool antibot,
                                   float range, float fov, std::vector<Player *> &playerList) {
  if (&player == localPlayer) {
    return true;
  }
  if (isBot(&player) && antibot) {
    return true;
  }
  const float distance = localPlayer->getDistance(&player);
  if (distance > range) {
    return true;
  }
  if (!isInFov(localPlayer, &player, fov)) {
    return true;
  }
  if (!player.isAlive() || player.getHealth() <= 0) {
    return true;
  }
  if (player.getName().empty()) {
    return true;
  }
  if (IsSelfTeam(&player)) {
    return true;
  }
  playerList.push_back(&player);
  return true;
}
bool Helper::Target::isInFov(LocalPlayer *player, Player *target, float maxfov) {
  if (maxfov >= 360.0F) {
    return true;
  }
  glm::vec3 localPos = player->getPosition();
  glm::vec3 targetPos = target->getPosition();
  glm::vec3 direction = glm::normalize(glm::vec3(targetPos.x - localPos.x,
                                                 0, // 忽略Y轴差异
                                                 targetPos.z - localPos.z));
  float targetYaw = glm::degrees(atan2(direction.z, direction.x)) - 90.0F;
  if (targetYaw < 0.0F) {
    targetYaw += 360.0F;
  }
  float localYaw = player->getYaw();
  float angleDiff = fabs(localYaw - targetYaw);
  angleDiff = fmod(angleDiff + 180.0F, 360.0F) - 180.0F;
  return fabs(angleDiff) <= maxfov / 2.0F;
}