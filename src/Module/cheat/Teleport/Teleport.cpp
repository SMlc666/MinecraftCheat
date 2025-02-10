#include "Teleport.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "game/minecraft/world/level/dimension/dimension.hpp"
#include "imgui/imgui.h"
#include "runtimes/runtimes.hpp"
#include <unordered_map>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"offestX", 0.0f},
    {"offestY", 0.0f},  {"offestZ", 0.0f},
};
}
cheat::Teleport::Teleport() : Module("Teleport", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnEnable([](Module *module) {
    try {
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;
      glm::vec3 offest =
          glm::vec3(module->getGUI().Get<float>("offestX"), module->getGUI().Get<float>("offestY"),
                    module->getGUI().Get<float>("offestZ"));
      player->setPosition(player->getPosition() + offest);
      module->getGUI().Set("enabled", false);
    } catch (...) {
      return;
    }
  });
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("offestX", "X轴偏移", -40.0F, 40.0F);
    module->getGUI().SliderFloat("offestY", "Y轴偏移", -40.0F, 40.0F);
    module->getGUI().SliderFloat("offestZ", "Z轴偏移", -40.0F, 40.0F);
    if (ImGui::TreeNode("Players")) {
      ClientInstance *instance = runtimes::getClientInstance();
      if (instance) {
        LocalPlayer *player = instance->getLocalPlayer();
        if (player) {
          Dimension *dimension = player->mDimension;
          if (dimension) {
            std::vector<Player *> players;
            dimension->forEachPlayer([&players](Player &p) {
              players.push_back(&p);
              return false;
            });
            for (auto *mplayer : players) {
              if (ImGui::Button(mplayer->getName().c_str())) {
                player->setPosition(mplayer->getPosition());
              }
            }
          }
        }
      }
      ImGui::TreePop();
    }
  });
}