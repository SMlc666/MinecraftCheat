#include "draw.hpp"
#include "dobby.h"
#include "my_imgui.h"
int g_GlHeight, g_GlWidth = 0;
bool is_ImguiSetup = false;
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);
  if (!is_ImguiSetup) {
    imguiSetup();
    is_ImguiSetup = true;
  }
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
  ImGui::NewFrame();
  ImGui::Begin("Minecraft Cheat");
  ImGui::End();
  ImGui::EndFrame();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  return old_eglSwapBuffers(dpy, surface);
}
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
void drawSetup() {
  void *egl_handle = dlopen("libEGL.so", 4);
  void *eglSwapBuffers = dlsym(egl_handle, "eglSwapBuffers");
  DobbyHook((void *)eglSwapBuffers, (void *)my_eglSwapBuffers, (void **)&old_eglSwapBuffers);
}