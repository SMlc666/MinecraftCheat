#include "Hud.hpp"

#include <math.h>
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "menu/menu.hpp"
#include <any>
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false}, {"shortcut", false}, {"arraylist", false}};
namespace Arraylist {
ImU32 colorTop = IM_COL32(255, 255, 255, 255);
ImU32 colorBottom = IM_COL32(255, 255, 255, 255);
float fontSize = 16.0f;
ImVec2 padding(10.0f, 10.0f);
} // namespace Arraylist
cheat::Hud::Hud() : Module("Hud", MenuType::RENDER_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    gui.CheckBox("arraylist", "功能列表");
  });
  setOnDraw([](Module *module) {});
}