#include "Hud.hpp"
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "draw.hpp"
#include "gui/gui.hpp"
#include "imgui/imgui.h"
#include "menu/menu.hpp"

// 颜色插值函数
static ImU32 ColorLerp(ImU32 col1, ImU32 col2, float t) {
    ImVec4 a = ImColor(col1);
    ImVec4 b = ImColor(col2);
    return ImColor(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t,
                   a.w + (b.w - a.w) * t);
}

// 配置数据
static const std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},  {"shortcut", false},     {"arraylist", false},
    {"fontSize", 28.0f}, {"lineSpacing", 3.0f},   {"padding", GUI::Vec2(20.0f, 20.0f)},
    {"bgAlpha", 0.85f},  {"gradientSpeed", 1.5f}, {"shadowIntensity", 0.7f},
    {"decoLines", true}, {"sortByLength", true},  {"onlyActive", true},
};

// 模块列表配置
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

// 通知系统配置
namespace Notifications {
float duration = 3.0f;        // 总显示时间
float fadeTime = 0.5f;        // 渐入/渐出时间
float spacing = 8.0f;         // 通知间距
ImVec2 margin{35.0f, 35.0f};  // 屏幕边距（右边距，下边距）
ImVec2 bgPadding{20.0f, 12.0f}; // 背景内边距
bool enableSlide = true;
float slideSpeed = 0.15f;
bool enableScale = true;
float maxScale = 1.8f;
ImVec4 bgColor{0.1f, 0.1f, 0.1f, 0.9f};
float cornerRounding = 8.0f;  // 圆角半径
}

// 模块显示信息
struct ModuleDisplayInfo {
    std::string name;
    ImVec2 textSize;
    float activateTime;
    size_t originalIndex;
};

// 通知结构体
struct Notification {
    std::string message;
    float startTime;
    float duration;
    ImU32 color;
    float animProgress; // 0-1进场 1-2退场
    float verticalOffset;
    float targetY;
};

// 自定义排序函数
bool SortModules(const ModuleDisplayInfo &a, const ModuleDisplayInfo &b) {
    if (Arraylist::sortByLength) {
        if (a.name.length() == b.name.length())
            return a.activateTime > b.activateTime;
        return a.name.length() > b.name.length();
    }
    return a.activateTime > b.activateTime;
}

// 自定义颜色渐变函数（左右渐变）
static ImU32 HorizontalGradientColor(float xPos, float maxWidth) {
    float t = xPos / maxWidth; // 根据水平位置计算渐变比例
    return ImColor::HSV(t, 0.9f, 1.0f); // HSV颜色插值
}

// 收集模块信息
std::vector<ModuleDisplayInfo> CollectModuleInfo() {
    std::vector<ModuleDisplayInfo> modules;
    auto &moduleList = ModuleManager::getModules();
    size_t index = 0;

    for (auto &[name, module] : moduleList) {
        try {
            if (Arraylist::onlyActive && !module->getGUI().Get<bool>("enabled"))
                continue;
        } catch (...) {
            continue;
        }

        ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
        textSize.x *= Arraylist::fontSize / 16.0f;
        textSize.y *= Arraylist::fontSize / 16.0f;

        modules.push_back({name, textSize, static_cast<float>(ImGui::GetTime()), index++});
    }

    return modules;
}

// 绘制背景板
void DrawBackground(ImDrawList *drawList, float startX, float currentY, float maxWidth, float totalHeight) {
    const ImU32 bgColor = IM_COL32(0, 0, 0, static_cast<int>(Arraylist::bgAlpha * 200));
    const ImVec2 bgPadding(12.0f, 10.0f);

    drawList->AddRectFilled(
        ImVec2(startX - bgPadding.x, currentY - bgPadding.y),
        ImVec2(startX + maxWidth + bgPadding.x, currentY + totalHeight + bgPadding.y),
        bgColor, 12.0f, ImDrawFlags_RoundCornersAll);
}

// 绘制模块列表（左右渐变）
void DrawModuleList(ImDrawList *drawList, const std::vector<ModuleDisplayInfo> &modules, float startX, float currentY, float maxWidth) {
    const float scaledFontSize = Arraylist::fontSize;

    for (size_t i = 0; i < modules.size(); ++i) {
        const auto &m = modules[i];

        // 颜色分配（左右渐变）
        const ImU32 textColor = HorizontalGradientColor(startX, maxWidth);

        // 加强阴影效果
        if (Arraylist::shadowIntensity > 0.01f) {
            for (int s = 0; s < 5; ++s) {
                const float shadowOffset = s * 0.5f;
                const ImU32 shadowColor = IM_COL32(0, 0, 0, static_cast<int>(220 * Arraylist::shadowIntensity));

                drawList->AddText(ImGui::GetFont(), scaledFontSize,
                                  ImVec2(startX + shadowOffset, currentY + shadowOffset), shadowColor,
                                  m.name.c_str());
            }
        }

        // 绘制文本
        drawList->AddText(ImGui::GetFont(), scaledFontSize, ImVec2(startX, currentY), textColor, m.name.c_str());

        // 装饰线
        if (Arraylist::decoLines) {
            const float lineLength = m.textSize.x * 0.4f + 10.0f * sinf(ImGui::GetTime() * 2 + i);
            drawList->AddLine(ImVec2(startX - 15.0f, currentY + m.textSize.y / 2),
                              ImVec2(startX - 15.0f + lineLength, currentY + m.textSize.y / 2),
                              textColor, 3.0f); // 使用渐变颜色
        }

        currentY += m.textSize.y + Arraylist::lineSpacing;
    }
}

// 通知管理器
class NotificationManager {
public:
    static NotificationManager& Get() {
        static NotificationManager instance;
        return instance;
    }

    void Add(const std::string& message, ImU32 color = IM_COL32(255, 255, 255, 255)) {
        // 计算新通知的尺寸
        ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
        float totalHeight = textSize.y + Notifications::bgPadding.y * 2;

        // 计算新通知的目标位置
        float yPos = static_cast<float>(g_GlHeight) - Notifications::margin.y;
        for (auto& n : notifications) {
            if (n.animProgress >= 1.0f) continue;
            ImVec2 currentTextSize = ImGui::CalcTextSize(n.message.c_str());
            float currentHeight = currentTextSize.y + Notifications::bgPadding.y * 2;
            yPos -= (currentHeight + Notifications::spacing);
        }

        // 添加新通知
        notifications.push_back({
            message,
            static_cast<float>(ImGui::GetTime()),
            Notifications::duration,
            color,
            0.0f,
            yPos,
            yPos
        });
    }

    void Draw(ImDrawList* drawList) {
        const float screenWidth = static_cast<float>(g_GlWidth);
        const float screenHeight = static_cast<float>(g_GlHeight);

        for (auto& n : notifications) {
            if (n.animProgress >= 2.0f) continue;

            const float animT = std::clamp(n.animProgress, 0.0f, 2.0f);
            const float alpha = std::min(animT, 2.0f - animT);
            const float slideOffset = (1.0f - alpha) * screenWidth * 0.2f;
            const float scale = Notifications::enableScale ?
                1.0f + (Notifications::maxScale - 1.0f) * (1.0f - std::abs(animT - 1.0f)) : 1.0f;

            // 颜色处理
            ImColor textColor(n.color);
            textColor.Value.w *= alpha;
            ImColor bgColor(Notifications::bgColor);
            bgColor.Value.w *= alpha * 0.9f;

            // 计算文本尺寸
            ImVec2 textSize = ImGui::CalcTextSize(n.message.c_str());
            textSize.x *= scale;
            textSize.y *= scale;

            // 计算背景尺寸
            const float bgWidth = textSize.x + Notifications::bgPadding.x * 2;
            const float bgHeight = textSize.y + Notifications::bgPadding.y * 2;

            // 垂直位置
            float verticalPos = n.verticalOffset;

            // 背景矩形位置
            const ImVec2 bgMin(
                screenWidth - bgWidth - Notifications::margin.x + slideOffset,
                verticalPos
            );
            const ImVec2 bgMax(
                screenWidth - Notifications::margin.x + slideOffset,
                verticalPos + bgHeight
            );

            // 绘制背景和文本
            drawList->AddRectFilled(bgMin, bgMax, bgColor, Notifications::cornerRounding);
            drawList->AddText(
                ImGui::GetFont(),
                ImGui::GetFontSize() * scale,
                ImVec2(bgMin.x + Notifications::bgPadding.x, bgMin.y + (bgHeight - textSize.y) / 2),
                textColor,
                n.message.c_str()
            );
        }
    }

    void Update() {
        const float currentTime = ImGui::GetTime();
        for (auto it = notifications.begin(); it != notifications.end();) {
            Notification& n = *it;
            const float elapsed = currentTime - n.startTime;

            // 动画阶段判断
            if (elapsed < Notifications::fadeTime) {
                n.animProgress = elapsed / Notifications::fadeTime;
            } else if (elapsed < n.duration - Notifications::fadeTime) {
                n.animProgress = 1.0f;
            } else {
                n.animProgress = 1.0f + (elapsed - (n.duration - Notifications::fadeTime)) / Notifications::fadeTime;
            }

            // 移除条件改为animProgress超过2.0f
            if (n.animProgress >= 2.0f) {
                it = notifications.erase(it);
            } else {
                ++it;
            }
        }

        // 重新计算所有通知的目标位置
        float yPos = static_cast<float>(g_GlHeight) - Notifications::margin.y;
        for (auto& n : notifications) {
            if (n.animProgress >= 1.0f) continue;

            ImVec2 textSize = ImGui::CalcTextSize(n.message.c_str());
            float totalHeight = textSize.y + Notifications::bgPadding.y * 2;
            n.targetY = yPos - totalHeight;
            yPos = n.targetY - Notifications::spacing;

            // 更新垂直位置
            n.verticalOffset += (n.targetY - n.verticalOffset) * Notifications::slideSpeed;
        }
    }

private:
    std::vector<Notification> notifications;
};

// 自动化通知系统
class AutoNotificationSystem {
public:
    static AutoNotificationSystem& Get() {
        static AutoNotificationSystem instance;
        return instance;
    }

    void Update() {
        CheckModuleStates();
        NotificationManager::Get().Update();
    }

    void Draw() {
        NotificationManager::Get().Draw(ImGui::GetBackgroundDrawList());
    }

private:
    std::unordered_map<std::string, bool> lastModuleStates;
    float lastCheckTime = 0.0f;

    void CheckModuleStates() {
        float currentTime = ImGui::GetTime();
        if (currentTime - lastCheckTime < 0.5f) return; // 每0.5秒检查一次
        lastCheckTime = currentTime;

        auto& moduleList = ModuleManager::getModules();
        for (auto& [name, module] : moduleList) {
            bool currentState = false;
            try {
                currentState = module->getGUI().Get<bool>("enabled");
            } catch (...) {
                continue;
            }

            // 检测状态变化
            if (lastModuleStates.find(name) != lastModuleStates.end()) {
                bool lastState = lastModuleStates[name];
                if (lastState != currentState) {
                    GenerateNotification(name, currentState);
                }
            }
            lastModuleStates[name] = currentState;
        }
    }

    void GenerateNotification(const std::string& moduleName, bool newState) {
        std::string message;
        ImU32 color;

        if (newState) {
            message = moduleName + " 已启用";
            color = IM_COL32(100, 255, 100, 255); // 绿色
        } else {
            message = moduleName + " 已禁用";
            color = IM_COL32(255, 100, 100, 255); // 红色
        }

        NotificationManager::Get().Add(message, color);
    }
};

// HUD模块实现
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

        // 收集模块信息
        auto modules = CollectModuleInfo();
        std::sort(modules.begin(), modules.end(), SortModules);

        // 计算布局
        float maxWidth = 0.0f;
        float totalHeight = 0.0f;
        for (const auto &m : modules) {
            maxWidth = std::max(maxWidth, m.textSize.x);
            totalHeight += m.textSize.y + Arraylist::lineSpacing;
        }

        const float Width = static_cast<float>(g_GlWidth);
        const float startX = Width - maxWidth - Arraylist::padding.x;
        float currentY = Arraylist::padding.y;

        // 绘制背景和模块列表
        if (!modules.empty()) {
            DrawBackground(drawList, startX, currentY, maxWidth, totalHeight);
            DrawModuleList(drawList, modules, startX, currentY, maxWidth);
        }

        // 自动通知系统
        static bool firstRun = true;
        if (firstRun) {
            NotificationManager::Get().Add("HUD 初始化完成", IM_COL32(100, 200, 255, 255));
            firstRun = false;
        }

        // 系统状态监控
        static float lastSystemNotify = 0.0f;
        float currentTime = ImGui::GetTime();
        if (currentTime - lastSystemNotify > 30.0f) {
            size_t activeCount = 0;
            for (auto& [name, module] : ModuleManager::getModules()) {
                try {
                    if (module->getGUI().Get<bool>("enabled")) activeCount++;
                } catch (...) {}
            }

            NotificationManager::Get().Add(
                "系统状态: " + std::to_string(activeCount) + "/" +
                std::to_string(ModuleManager::getModules().size()) + " 模块激活",
                IM_COL32(150, 150, 255, 255)
            );
            lastSystemNotify = currentTime;
        }

        // 更新并绘制通知
        AutoNotificationSystem::Get().Update();
        AutoNotificationSystem::Get().Draw();
    });
}