#include "draw.hpp"
#include <stdexcept>
#include "imgui/imgui.h"

static bool ScriptFrame = false;

void ImGui_ImplScript_NewFrame() {
  ScriptFrame = true;
}
void ImGui_ImplScript_EndFrame() {
  ScriptFrame = false;
}
void ScriptAPI::draw::Text(std::string &text) {
  if (!ScriptFrame) {
    throw std::runtime_error("Cannot call draw::Text outside of a script frame");
  }
  ImGui::Text("%s", text.c_str());
}