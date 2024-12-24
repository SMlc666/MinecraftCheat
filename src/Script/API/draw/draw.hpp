#pragma once
#include <string>

//在ImGui::NewFrame()之后调用,不暴露给Script

extern void ImGui_ImplScript_NewFrame();
//在ImGui::EndFrame()之后调用,不暴露给Script
extern void ImGui_ImplScript_EndFrame();
namespace ScriptAPI::draw {
void Text(std::string text); //绘制文本,不提供format参数
}
namespace ScriptAPI::draw::native {}
