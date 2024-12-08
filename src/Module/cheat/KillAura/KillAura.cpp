#include "KillAura.hpp"
#include "imgui.h"
// 定义一个模块
KillAuraModule::KillAuraModule(const std::string &name, MenuType type) : Module(name, type) {
  setOnDraw([](Module *self) {
    if (ImGui::TreeNodeEx("KillAura")) {
      ImGui::Text("KillAura");
    }
  });
}