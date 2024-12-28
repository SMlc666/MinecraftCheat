#include "draw.hpp"
#include "Dobby/dobby.h"
#include "iMsgCapture/iMsgCapture.h"
#include "log.hpp"
#include "my_imgui.h"
#include "API/draw/draw.hpp"
#include "ModuleManager.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <format>
static bool isMainMenuActivated = false;
void drawMenu(MenuType menuType) {
  std::string MenuName;
  if (!isMainMenuActivated && menuType != MAIN_MENU) {
    return; //主菜单未激活，其他菜单不绘制
  }
  try {
    MenuName = MenuNames.at(menuType);
  } catch (const std::out_of_range &e) {
    g_log_tool.message(LogLevel::ERROR, "drawMenu", "No such menu type" + std::string(e.what()));
    return; // 防止崩溃
  }
  bool menuActive = ImGui::Begin(MenuName.c_str()); // 使用数组中的字符串作为窗口标题
  if (menuType == MAIN_MENU) {
    isMainMenuActivated = menuActive;
  }
  if (!menuActive) {
    ImGui::End();
    return;
  }
  try {
    MenuFunctions.at(menuType)(); // 调用对应菜单的绘制函数
  } catch (const std::out_of_range &e) {
    g_log_tool.message(LogLevel::ERROR, "drawMenu",
                       "No such menu function" + std::string(e.what()));
    return; // 防止崩溃
  }
  for (const auto &pair : ModuleManager::getModules()) {
    if (pair.second && pair.second->getMenuType() == menuType) {
      pair.second->onDraw();
    }
  }
  ImGui::End();
}
// 主绘制函数，根据需要调用
void drawAllModules() {
  drawMenu(MAIN_MENU);
  drawMenu(LOG_MENU);
  drawMenu(SCRIPT_MENU);
  drawMenu(CONFIG_MENU);
  drawMenu(DEBUG_MENU);
}
int g_GlHeight, g_GlWidth = 0; //opengl窗口的高度和宽度
bool is_ImguiSetup = false;
static const std::string IniFile = "/sdcard/MinecraftCheat/imgui.ini";
static const float NormalFontSize = 22.0F;
static const float NormalScaleFactor = 3.0F;
static const int NormalInputUpdateInterval = 1000;
void imguiSetup() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &ioData = ImGui::GetIO();
  ioData.IniFilename = IniFile.c_str();
  ioData.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);
  ImGui_ImplAndroid_Init(nullptr);
  ImGui_ImplOpenGL3_Init("#version 300 es");
  ImGui::StyleColorsDark();
  ImGui::Android_LoadSystemFont(NormalFontSize);
  ImGui::GetStyle().ScaleAllSizes(NormalScaleFactor);
}
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);
  if (!is_ImguiSetup) {
    imguiSetup();
    is_ImguiSetup = true;
  }
  ImGuiIO &ioData = ImGui::GetIO();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
  ImGui::NewFrame();
  ImGui_ImplScript_NewFrame();
  ModuleManager::drawAllModules();
  ImGui::EndFrame();
  ImGui_ImplScript_NewFrame();
  ImGui::Render();
  glViewport(0, 0, (int)ioData.DisplaySize.x, (int)ioData.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return old_eglSwapBuffers(dpy, surface);
}
void (*old_Input)(void *thiz, void *ex_ab, void *ex_ac);
void my_Input(void *thiz, void *ex_ab, void *ex_ac) {
  // 调用原始函数
  old_Input(thiz, ex_ab, ex_ac);

  // 处理输入事件
  if (is_ImguiSetup) {
    ImGui_ImplAndroid_HandleInputEvent(static_cast<AInputEvent *>(thiz));
  }
}
void CaptureInput(iMsgEvent *iMsg) {
  if (is_ImguiSetup) {
    ImGui_ImplAndroid_HandleInputMsg(iMsg);
  }
}
void drawSetup() {
  void *egl_handle = dlopen("libEGL.so", 4);
  g_log_tool.message(LogLevel::INFO, "drawSetup", std::format("egl_handle: {:p}", egl_handle));
  if (egl_handle != nullptr) {
    void *eglSwapBuffers = dlsym(egl_handle, "eglSwapBuffers");
    if (eglSwapBuffers == nullptr) {
      g_log_tool.message(LogLevel::ERROR, "drawSetup", "eglSwapBuffers is null");
      return;
    }
    g_log_tool.message(LogLevel::INFO, "drawSetup",
                       std::format("eglSwapBuffers: {:p}", eglSwapBuffers));
    //NOLINTBEGIN
    DobbyHook(eglSwapBuffers, reinterpret_cast<void *>(my_eglSwapBuffers),
              reinterpret_cast<void **>(&old_eglSwapBuffers));
    //NOLINTEND
  } else {
    g_log_tool.message(LogLevel::ERROR, "drawSetup", "egl_handle is null");
  }
  void *sym_input = DobbySymbolResolver(
      nullptr,
      "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
  if (sym_input != nullptr) {
    g_log_tool.message(LogLevel::INFO, "drawSetup", std::format("sym_input: {:p}", sym_input));
    //NOLINTBEGIN
    DobbyHook(sym_input, reinterpret_cast<void *>(my_Input), reinterpret_cast<void **>(&old_Input));
    //NOLINTEND
  } else {
    g_log_tool.message(LogLevel::ERROR, "drawSetup", "sym_input is null");
    iMsgCapture::instance().setCallback(CaptureInput);
    iMsgCapture::instance().setUpdateInterval(NormalInputUpdateInterval);
  }
}