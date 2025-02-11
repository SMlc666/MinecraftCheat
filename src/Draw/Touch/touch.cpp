#include "touch.hpp"
#include "MemTool.hpp"
#include "imgui/backends/imgui_impl_android.h"
#include "draw.hpp"
#include "log.hpp"
#include <string>
std::mutex imgui_input_mutex;
static void (*old_Input)(void *thiz, void *ex_ab, void *ex_ac);
static void my_Input(void *thiz, void *ex_ab, void *ex_ac) {
  // 调用原始函数
  old_Input(thiz, ex_ab, ex_ac);
  // 处理输入事件
  if (is_ImguiSetup) {
    std::lock_guard<std::mutex> lock(imgui_input_mutex);
    ImGui_ImplAndroid_HandleInputEvent(static_cast<AInputEvent *>(thiz));
  }
}

void touchSetup() {

  //NOLINTBEGIN
  try {
    MemTool::Hook(
        "libinput.so",
        "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE",
        reinterpret_cast<void *>(my_Input), reinterpret_cast<void **>(&old_Input), false);
  } catch (const std::exception &e) {
    g_log_tool.message(LogLevel::ERROR, "Touch",
                       "Failed to hook InputConsumer::initializeMotionEvent: " +
                           std::string(e.what()))
  }
  //NOLINTEND
}