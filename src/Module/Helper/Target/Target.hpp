#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
namespace Helper::Target {
bool isInFov(LocalPlayer *player, Player *target, float maxfov);
bool isBot(Player *player);
bool ProcessPlayer(Player &player, LocalPlayer *localPlayer, bool antibot, float range, float fov,
                   std::vector<Player *> &playerList);
bool hasPlayer(Dimension *dimension, Player *targetPlayer, LocalPlayer *localPlayer, bool antibot,
               float range, float fov);
} // namespace Helper::Target