#include "touch.hpp"
#include "Dobby/dobby.h"
#include "imgui/backends/imgui_impl_android.h"
#include "draw.hpp"
#include "log.hpp"
#include "iMsgCapture/iMsgCapture.h"
#include <string>
#include <vector>
static const int NormalInputUpdateInterval = 1000;
static const std::vector<std::string> NormalGameActivityName = {
    "com.mojang.minecraftpe.MainActivity"};
static void (*old_Input)(void *thiz, void *ex_ab, void *ex_ac);
static void my_Input(void *thiz, void *ex_ab, void *ex_ac) {
  // 调用原始函数
  old_Input(thiz, ex_ab, ex_ac);

  // 处理输入事件
  if (is_ImguiSetup) {
    ImGui_ImplAndroid_HandleInputEvent(static_cast<AInputEvent *>(thiz));
  }
}
static void CaptureInput(iMsgEvent *iMsg) {
  if (is_ImguiSetup) {
    ImGui_ImplAndroid_HandleInputMsg(iMsg);
  }
}

void touchSetup() {
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
    iMsgCapture::instance().runThread(NormalGameActivityName, true, 1);
  }
}