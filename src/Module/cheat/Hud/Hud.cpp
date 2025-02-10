#include "Hud.hpp"
#include <math.h>
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "draw.hpp"
#include "gui/gui.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"
#include <any>
#include <string>
#include <unordered_map>
#include <algorithm>

static ImU32 ColorLerp(ImU32 col1, ImU32 col2, float t) {
  ImVec4 a = ImColor(col1);
  ImVec4 b = ImColor(col2);
  return ImColor(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t,
                 a.w + (b.w - a.w) * t);
}

static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},  {"shortcut", false},     {"arraylist", false},
    {"fontSize", 28.0f}, {"lineSpacing", 3.0f},   {"padding", GUI::Vec2(20.0f, 20.0f)},
    {"bgAlpha", 0.85f},  {"gradientSpeed", 1.5f}, {"shadowIntensity", 0.7f},
    {"decoLines", true}, {"sortByLength", true},  {"onlyActive", true},
};

namespace Arraylist {
float fontSize = 28.0f;
float lineSpacing = 3.0f;
ImVec2 padding(20.0f, 20.0f);
float bgAlpha = 0.85f;
float gradientSpeed = 1.5f;
float shadowIntensity = 0.7f;
bool decoLines = false;
bool sortByLength = true;
bool onlyActive = true;
} // namespace Arraylist

struct ModuleDisplayInfo {
  std::string name;
  ImVec2 textSize;
  float activateTime;
  size_t originalIndex;
};

// 自定义排序函数
bool SortModules(const ModuleDisplayInfo &a, const ModuleDisplayInfo &b) {
  if (Arraylist::sortByLength) {
    // 按长度降序排列，长度相同则按激活时间排序
    if (a.name.length() == b.name.length())
      return a.activateTime > b.activateTime;
    return a.name.length() > b.name.length();
  }
  // 默认按激活时间排序
  return a.activateTime > b.activateTime;
}

cheat::Hud::Hud() : Module("Hud", MenuType::RENDER_MENU, ConfigData) {
  setOnDrawGUI([](Module *module) {
    auto &gui = module->getGUI();
    if (ImGui::TreeNode("高级显示设置")) {
      gui.CheckBox("arraylist", "功能列表");

      ImGui::SeparatorText("布局设置");
      gui.SliderFloat("fontSize", "字体大小", 12.0F, 48.0F,
                      [](float value) { Arraylist::fontSize = value; });
      gui.SliderFloat("lineSpacing", "行间距", -10.0F, 10.0F,
                      [](float value) { Arraylist::lineSpacing = value; });
      gui.SliderFloat2("padding", "边距", -50.0F, 50.0F,
                       [](float x, float y) { Arraylist::padding = ImVec2(x, y); });

      ImGui::SeparatorText("视觉效果");
      gui.SliderFloat("gradientSpeed", "渐变速度", 0.1f, 3.0f,
                      [](float value) { Arraylist::gradientSpeed = value; });
      gui.SliderFloat("bgAlpha", "背景透明度", 0.0f, 1.0f,
                      [](float value) { Arraylist::bgAlpha = value; });
      gui.SliderFloat("shadowIntensity", "阴影强度", 0.0f, 1.0f,
                      [](float value) { Arraylist::shadowIntensity = value; });
      gui.CheckBox("decoLines", "装饰线", [](bool value) { Arraylist::decoLines = value; });

      ImGui::SeparatorText("排序设置");
      gui.CheckBox("sortByLength", "按长度排序",
                   [](bool value) { Arraylist::sortByLength = value; });
      gui.CheckBox("onlyActive", "只显示激活模块",
                   [](bool value) { Arraylist::onlyActive = value; });

      ImGui::TreePop();
    }
  });

  setOnLoad([](Module *module) {
    auto &gui = module->getGUI();
    try {
      Arraylist::fontSize = gui.Get<float>("fontSize");
      Arraylist::lineSpacing = gui.Get<float>("lineSpacing");
      Arraylist::padding = ImVec2(gui.GetVec2("padding").x, gui.GetVec2("padding").y);
      Arraylist::bgAlpha = gui.Get<float>("bgAlpha");
      Arraylist::gradientSpeed = gui.Get<float>("gradientSpeed");
      Arraylist::shadowIntensity = gui.Get<float>("shadowIntensity");
      Arraylist::decoLines = gui.Get<bool>("decoLines");
      Arraylist::sortByLength = gui.Get<bool>("sortByLength");
      Arraylist::onlyActive = gui.Get<bool>("onlyActive");
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

    if (!arraylist)
      return;

    ImDrawList *drawList = ImGui::GetBackgroundDrawList();
    const float fontScale = Arraylist::fontSize / 16.0f;
    const float scaledFontSize = Arraylist::fontSize;
    static float globalHue = 0.0f;
    globalHue = fmod(ImGui::GetTime() * 0.2f * Arraylist::gradientSpeed, 1.0f);

    // 收集模块信息
    std::vector<ModuleDisplayInfo> modules;
    auto &moduleList = ModuleManager::getModules();
    float maxWidth = 0.0f;
    size_t index = 0;

    for (auto &[name, module] : moduleList) {
      try {
        if (Arraylist::onlyActive && !module->getGUI().Get<bool>("enabled"))
          continue;
      } catch (...) {
        continue;
      }

      ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
      textSize.x *= fontScale;
      textSize.y *= fontScale;
      maxWidth = std::max(maxWidth, textSize.x);

      modules.push_back({name, textSize, static_cast<float>(ImGui::GetTime()), index++});
    }

    // 排序处理
    std::sort(modules.begin(), modules.end(), SortModules);

    // 布局计算
    const float Width = static_cast<float>(g_GlWidth);
    const float Height = static_cast<float>(g_GlHeight);
    const float startX = Width - maxWidth - Arraylist::padding.x;
    float currentY = Arraylist::padding.y;

    // 绘制背景板
    if (modules.size() > 0) {
      float totalHeight = 0.0f;
      for (const auto &m : modules)
        totalHeight += m.textSize.y + Arraylist::lineSpacing;

      const ImU32 bgColor = IM_COL32(0, 0, 0, static_cast<int>(Arraylist::bgAlpha * 200));
      const ImVec2 bgPadding(12.0f, 10.0f);

      drawList->AddRectFilled(
          ImVec2(startX - bgPadding.x, currentY - bgPadding.y),
          ImVec2(Width - Arraylist::padding.x + bgPadding.x, currentY + totalHeight + bgPadding.y),
          bgColor, 12.0f, ImDrawFlags_RoundCornersAll);
    }

    // 动态渐变绘制
    for (size_t i = 0; i < modules.size(); ++i) {
      const auto &m = modules[i];

      // 颜色计算
      const float hueOffset = static_cast<float>(m.originalIndex) * 0.03f;
      const ImU32 textColor = ImColor::HSV(fmod(globalHue + hueOffset, 1.0f), 0.85f, 1.0f);

      // 阴影效果
      if (Arraylist::shadowIntensity > 0.01f) {
        const ImU32 shadowColor =
            IM_COL32(0, 0, 0, static_cast<int>(200 * Arraylist::shadowIntensity));
        for (int s = 0; s < 3; ++s) {
          drawList->AddText(ImGui::GetFont(), scaledFontSize,
                            ImVec2(startX + s * 0.7f, currentY + s * 0.7f), shadowColor,
                            m.name.c_str());
        }
      }

      // 主文字
      drawList->AddText(ImGui::GetFont(), scaledFontSize, ImVec2(startX, currentY), textColor,
                        m.name.c_str());

      // 装饰线
      if (Arraylist::decoLines) {
        const float lineLength = m.textSize.x * 0.4f + 10.0f * sinf(ImGui::GetTime() * 2 + i);
        drawList->AddLine(ImVec2(startX - 15.0f, currentY + m.textSize.y / 2),
                          ImVec2(startX - 15.0f + lineLength, currentY + m.textSize.y / 2),
                          ImColor::HSV(fmod(globalHue + 0.3f, 1.0f), 0.7f, 1.0f), 3.0f);
      }

      currentY += m.textSize.y + Arraylist::lineSpacing;
    }
  });
}