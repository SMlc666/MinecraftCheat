#pragma once
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/actor/player/player.hpp"
#include "game/minecraft/actor/player/gamemode/gamemode.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/minecraft.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "game/minecraft/world/level/block/BlockSource.hpp"
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
    ImGui::SameLine();
    if (ImGui::Button("get Position")) {
      if (localPlayer != nullptr) {
        auto pos = localPlayer->getPosition();
        g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                           std::format("LocalPlayer Position: {} , {} , {}", pos.x, pos.y, pos.z));
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Attack self")) {
      if (localPlayer != nullptr) {
        localPlayer->getGameMode().attack(*localPlayer);
      }
    }
    if (ImGui::TreeNode("Dimension")) {
      Dimension *dimension{};
      if (localPlayer != nullptr) {
        dimension = localPlayer->mDimension;
      }
      if (ImGui::Button("getDimension")) {
        if (dimension != nullptr) {
          g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                             std::format("Dimension: {:p}", reinterpret_cast<void *>(dimension)));
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("getNormalDimension")) {
        if (dimension != nullptr) {
          g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                             std::format("Normal Dimension: {}",
                                         reinterpret_cast<void *>(&localPlayer->mDimension)));
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("forEachPlayer")) {
        if (dimension != nullptr) {
          dimension->forEachPlayer([](Player &player) {
            g_log_tool.message(LogLevel::DEBUG, "DEBUG", "forEachPlayer");
            g_log_tool.message(LogLevel::DEBUG, "DEBUG", player.getName());
            return true;
          });
        }
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("BlockSource")) {
      BlockSource *blockSource{};
      if (instance != nullptr) {
        blockSource = instance->getRegion();
      }
      if (ImGui::Button("get block source")) {
        if (blockSource != nullptr) {
          g_log_tool.message(
              LogLevel::DEBUG, "DEBUG",
              std::format("BlockSource: {:p}", reinterpret_cast<void *>(blockSource)));
        }
      }
    }
    if (ImGui::TreeNode("ClientInstance")) {
      if (ImGui::Button("get client instance")) {
        if (instance != nullptr) {
          g_log_tool.message(
              LogLevel::DEBUG, "DEBUG",
              std::format("ClientInstance: {:p}", reinterpret_cast<void *>(instance)));
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("get minecraft")) {
        if (instance != nullptr) {
          Minecraft *minecraft = instance->minecraftPtr;
          if (minecraft != nullptr) {
            g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                               std::format("Minecraft: {:p}", reinterpret_cast<void *>(minecraft)));
          }
        }
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Minecraft")) {
      if (ImGui::Button("getSimPaused")) {
        if (instance != nullptr) {
          Minecraft *minecraft = instance->minecraftPtr;
          if (minecraft != nullptr) {
            g_log_tool.message(LogLevel::DEBUG, "DEBUG",
                               std::format("getSimPaused: {}", minecraft->getSimPaused()));
          }
        }
      }
      ImGui::TreePop();
    }
    ImGui::TreePop();
  }
}