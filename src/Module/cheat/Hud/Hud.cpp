#include "Hud.hpp"

#include <math.h>
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "draw.hpp"
#include "gui/gui.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "menu/menu.hpp"
#include <any>
#include <string>
#include <unordered_map>
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"shortcut", false},
    {"arraylist", false},
    {"fontSize", 16.0f},
    {"lineSpacing", 2.0f},
    {"padding", GUI::Vec2(10.0f, 10.0f)},
    {"colorTop", GUI::Color(255, 255, 255, 255)},
    {"onlyActive", true},
};

namespace Arraylist {
ImU32 colorTop = IM_COL32(255, 255, 255, 255);
float fontSize = 16.0f;
float lineSpacing = 2.0f;
ImVec2 padding(10.0f, 10.0f);
bool onlyActive = true;
} // namespace Arraylist
struct Array {
  std::string name;
  ImVec2 textSize;
};
cheat::Hud::Hud() : Module("Hud", MenuType::RENDER_MENU, ConfigData) {
  setOnEnable([](Module *module) {});
  setOnDisable([](Module *module) {});
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    if (ImGui::TreeNode("Arraylist")) {
      gui.CheckBox("arraylist", "功能列表");
      gui.SliderFloat("fontSize", "字体大小", 1.0F, 44.0F,
                      [](float value) { Arraylist::fontSize = value; });
      gui.SliderFloat("lineSpacing", "行间距", 1.0F, 10.0F,
                      [](float value) { Arraylist::lineSpacing = value; });
      gui.SliderFloat2("padding", "边距", 0.0F, 30.0F,
                       [](float x, float y) { Arraylist::padding = ImVec2(x, y); });
      gui.ColorEdit("colorTop", "字体颜色", [](GUI::Color color) {
        Arraylist::colorTop = IM_COL32(color.r, color.g, color.b, color.a);
      });
      gui.CheckBox("onlyActive", "只显示激活模块",
                   [](bool value) { Arraylist::onlyActive = value; });
      ImGui::TreePop();
    }
  });
  setOnLoad([](Module *module) {
    auto &gui = module->getGUI();
    try {
      auto fontSize = gui.Get<float>("fontSize");
      auto lineSpacing = gui.Get<float>("lineSpacing");
      GUI::Color colorTop = gui.GetColor("colorTop");
      auto padding = gui.GetVec2("padding");
      bool onlyActive = gui.Get<bool>("onlyActive");
      Arraylist::fontSize = fontSize;
      Arraylist::lineSpacing = lineSpacing;
      Arraylist::colorTop = IM_COL32(colorTop.r, colorTop.g, colorTop.b, colorTop.a);
      Arraylist::padding = ImVec2(padding.x, padding.y);
      Arraylist::onlyActive = onlyActive;
    } catch (...) {
      return;
    }
  });
  setOnDraw([](Module *module) {
    auto &gui = module->getGUI();
    bool arraylist{};
    try {
      arraylist = gui.Get<bool>("arraylist");
    } catch (...) {
      return;
    }

    if (!arraylist) {
      return;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();

    // 设置字体大小
    float fontScale = Arraylist::fontSize / 16.0f; // ImGui的默认字体大小通常是16
    float scaledFontSize = Arraylist::fontSize;

    std::vector<Array> boundsList;
    auto &moduleList = ModuleManager::getModules();

    float maxWidth = 0.0f;
    float totalHeight = 0.0f;

    // 计算所有文本的边界
    for (const auto &module : moduleList) {
      try {
        if (Arraylist::onlyActive && !module.second->getGUI().Get<bool>("enabled")) {
          continue;
        }
      } catch (...) {
        continue;
      }
      ImVec2 textSize = ImGui::CalcTextSize(module.first.c_str());
      textSize.x *= fontScale;
      textSize.y *= fontScale;

      maxWidth = std::max(maxWidth, textSize.x);
      totalHeight += textSize.y + Arraylist::lineSpacing;

      boundsList.push_back(Array{module.first, textSize});
    }

    // 计算起始位置
    auto Width = static_cast<float>(g_GlWidth);
    auto Height = static_cast<float>(g_GlHeight);

    float startX = Width - maxWidth - Arraylist::padding.x;
    float startY = Arraylist::padding.y;

    // 确保起始位置在屏幕内
    startX =
        std::max(Arraylist::padding.x, std::min(startX, Width - maxWidth - Arraylist::padding.x));

    // 绘制每个模块名称
    float currentY = startY;
    for (const auto &item : boundsList) {
      // 确保不会超出屏幕底部
      if (currentY + item.textSize.y > Height - Arraylist::padding.y) {
        break;
      }

      // 绘制文本
      drawList->AddText(ImGui::GetFont(), scaledFontSize, ImVec2(startX, currentY),
                        Arraylist::colorTop, item.name.c_str());

      // 更新Y位置
      currentY += item.textSize.y + Arraylist::lineSpacing;
    }
  });
}