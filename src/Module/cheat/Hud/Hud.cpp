#include "Hud.hpp"

#include <math.h>
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "draw.hpp"
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
float lineSpacing = 2.0f;
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
    gui.CheckBox("arraylist", "功能列表");
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