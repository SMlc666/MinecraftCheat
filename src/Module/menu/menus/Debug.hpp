#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/Level.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "imgui/imgui.h"
#include "runtimes/runtimes.hpp"
#include "log.hpp"
#include <cstdlib>
#include <format>
#include <stdexcept>
void drawDebugMenu() {
  if (ImGui::TreeNode("Crash Test")) {
    if (ImGui::Button("Test Crash Abort")) {
      std::abort();
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash Throw")) {
      throw std::runtime_error("Test Crash Throw");
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash Null Pointer")) {
      int *ptr = nullptr;
      *ptr = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Crash SIGSEGV")) {
      *(int *)0 = 0; //NOLINT
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("LocalPlayer")) {
    ClientInstance *instance = runtimes::getClientInstance();
    LocalPlayer *localPlayer{};
    if (instance != nullptr) {
      localPlayer = instance->getLocalPlayer();
    }
    if (ImGui::Button("get localplayer")) {
      if (localPlayer != nullptr) {
        g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                           std::format("LocalPlayer: {:p}", reinterpret_cast<void *>(localPlayer)));
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("get Name")) {
      if (localPlayer != nullptr) {
        g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                           std::format("LocalPlayer Name: {}", localPlayer->getName()));
      }
    }
    if (ImGui::TreeNode("Level")) {
      Level *level = nullptr;
      if (localPlayer != nullptr) {
        level = localPlayer->getLevel();
      }
      if (ImGui::Button("get level")) {
        if (level != nullptr) {
          g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                             std::format("Level: {:p}", reinterpret_cast<void *>(level)));
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("forEachPlayer")) {
        if (level != nullptr) {
          std::function mCallback = [](Player &player) {
            g_log_tool.message(
                LogLevel::DEBUG, "DEBUG",
                std::format("Player address: {:p}", reinterpret_cast<void *>(&player)));
            return true;
          };
          level->forEachPlayer(&mCallback);
        }
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Dimension")) {
      Dimension *dimension = nullptr;
      if (localPlayer != nullptr) {
        dimension = localPlayer->getDimension();
      }
      if (ImGui::Button("get dimension")) {
        if (dimension != nullptr) {
          g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                             std::format("Dimension: {:p}", reinterpret_cast<void *>(dimension)));
        }
      }
      if (ImGui::Button("forEachPlayer")) {
        if (dimension != nullptr) {
          std::function mCallback = [](Player &player) {
            g_log_tool.message(
                LogLevel::DEBUG, "DEBUG",
                std::format("Player address: {:p}", reinterpret_cast<void *>(&player)));
            return true;
          };
          dimension->forEachPlayer(mCallback);
        }
      }
      ImGui::TreePop();
    }
    ImGui::TreePop();
  }
}