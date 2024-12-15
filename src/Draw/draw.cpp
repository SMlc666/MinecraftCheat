#include "draw.hpp"
#include "Dobby/dobby.h"
#include "log.hpp"
#include "my_imgui.h"
#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <format>
#include "main.hpp"
int g_GlHeight, g_GlWidth = 0; //opengl窗口的高度和宽度
bool is_ImguiSetup = false;
static const std::string IniFile = "/sdcard/MinecraftCheat/config.ini";
void imguiSetup() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);
  ImGui_ImplAndroid_Init(nullptr);
  ImGui_ImplOpenGL3_Init("#version 300 es");
  ImGui::StyleColorsLight();
  ImGui::Android_LoadSystemFont(22.0f);
  ImGui::GetStyle().ScaleAllSizes(3.0f);
}
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);
  if (!is_ImguiSetup) {
    imguiSetup();
    is_ImguiSetup = true;
  }
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
  ImGui::NewFrame();
  ModuleManager::drawAllModules();
  ImGui::EndFrame();
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return old_eglSwapBuffers(dpy, surface);
}
void (*old_Input)(void *thiz, void *ex_ab, void *ex_ac);
void my_Input(void *thiz, void *ex_ab, void *ex_ac) {
  // 调用原始函数
  old_Input(thiz, ex_ab, ex_ac);

  // 处理输入事件
  if (is_ImguiSetup) {
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
  }
  return;
}
void drawSetup() {
  void *egl_handle = dlopen("libEGL.so", 4);
  g_log_tool.message(LogLevel::INFO, "drawSetup", std::format("egl_handle: {:p}", egl_handle));
  if (egl_handle != nullptr) {
    void *eglSwapBuffers = dlsym(egl_handle, "eglSwapBuffers");
    g_log_tool.message(LogLevel::INFO, "drawSetup",
                       std::format("eglSwapBuffers: {:p}", eglSwapBuffers));
    DobbyHook((void *)eglSwapBuffers, (void *)my_eglSwapBuffers, (void **)&old_eglSwapBuffers);
  }
  void *sym_input = DobbySymbolResolver(
      nullptr,
      "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
  if (sym_input != nullptr) {
    g_log_tool.message(LogLevel::INFO, "drawSetup", std::format("sym_input: {:p}", sym_input));
    DobbyHook((void *)sym_input, (void *)my_Input, (void **)&old_Input);
  }
}