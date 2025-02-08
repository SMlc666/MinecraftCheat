#include "JetPack.hpp"
#include "Module.hpp"
#include "game/minecraft/actor/player/localplayer.hpp"
#include "game/minecraft/client/instance/clientinstance.hpp"
#include "menu/menu.hpp"
#include "runtimes/runtimes.hpp"
#include <any>
#include <string>
#include <unordered_map>
#include <vector>
namespace {
const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"speed", 1.0F}, {"ChangeY", false}, {"Mode", 0},
};
const std::vector<std::string> ModeItems = {"Motion", "setPos"};
} // namespace
cheat::JetPack::JetPack() : Module("JetPack", MenuType::MOVEMENT_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) {
    module->getGUI().SliderFloat("speed", "速度", 0.1F, 10.0F);
    module->getGUI().CheckBox("ChangeY", "Y轴方向");
    module->getGUI().Selectable("Mode", "模式", ModeItems);
  });
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnTick([](Module *module) {
    try {
      float speed = module->getGUI().Get<float>("speed");
      bool ChangeY = module->getGUI().Get<bool>("ChangeY");
      int Mode = module->getGUI().Get<int>("Mode");
      ClientInstance *instance = runtimes::getClientInstance();
      if (!instance)
        return;
      LocalPlayer *player = instance->getLocalPlayer();
      if (!player)
        return;
      float yaw = player->getYaw();
      float pitch = player->getPitch();
      float yawRad = yaw * 3.14159265F / 180.0F;
      float pitchRad = pitch * 3.14159265F / 180.0F;
      glm::vec3 direction;
      direction.x = -sinf(yawRad) * cosf(pitchRad);
      direction.z = cosf(yawRad) * cosf(pitchRad);
      direction.y = -sinf(pitchRad);
      glm::vec3 move = direction * (speed / 20.0F);
      if (!ChangeY) {
        move.y = 0.0F;
      }
      if (Mode == 0) {
        player->setMotion(move);

      } else if (Mode == 1) {
        glm::vec3 pos = player->getPosition();
        pos += move;
        player->setPosition(pos);
      }
    } catch (...) {
      return;
    }
  });
}