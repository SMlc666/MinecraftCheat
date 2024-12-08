#include "draw.hpp"
#include "dobby.h"
#include "my_imgui.h"
int g_GlHeight, g_GlWidth = 0;
bool is_ImguiSetup = false;
void imguiSetup() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.DisplaySize = ImVec2(1280, 720);
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
  ImGui::Begin("Minecraft Cheat");
  ImGui::End();
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
  ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);

  return;
}

void drawSetup() {
  void *egl_handle = dlopen("libEGL.so", 4);
  void *eglSwapBuffers = dlsym(egl_handle, "eglSwapBuffers");
  DobbyHook((void *)eglSwapBuffers, (void *)my_eglSwapBuffers, (void **)&old_eglSwapBuffers);
  void *sym_input = DobbySymbolResolver(
      nullptr,
      "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
  if (sym_input != nullptr) {
    //DobbyHook((void *)sym_input, (void *)my_Input, (void **)&old_Input);
  }
}