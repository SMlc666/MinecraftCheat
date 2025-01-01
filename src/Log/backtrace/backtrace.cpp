#include "log.hpp"
#include "backtrace.hpp"
#include <dlfcn.h>
#include <format>
#include <unwind.h>
static const int MAX_FRAMES = 100;
// 回调函数，用于处理每个栈帧
static _Unwind_Reason_Code backtrace_callback(struct _Unwind_Context *context, void *arg) {
  static int frame_count = 0;
  if (frame_count >= MAX_FRAMES) {
    return _URC_END_OF_STACK;
  }

  // 获取指令指针（IP）
  std::uintptr_t instructionPointer = _Unwind_GetIP(context);
  if (instructionPointer != 0U) {
    // 解析符号信息
    Dl_info info;
    //NOLINTNEXTLINE
    if ((dladdr(reinterpret_cast<void *>(instructionPointer), &info) != 0) &&
        (info.dli_sname != nullptr)) {
      g_log_tool.message(
          LogLevel::FATAL, "backtrace",
          std::format("#{} 0x{:x} {} + {}", frame_count, instructionPointer, info.dli_sname,
                      instructionPointer - reinterpret_cast<std::uintptr_t>(info.dli_saddr)));
    } else {
      g_log_tool.message(LogLevel::FATAL, "backtrace",
                         std::format("#{} 0x{:x} <unknown>", frame_count, instructionPointer));
    }
    frame_count++;
  }

  return _URC_NO_REASON;
}
static void signalhandler(int sig) {
  // 打印栈回溯
  _Unwind_Backtrace(backtrace_callback, nullptr);
  std::terminate();
}

void backtraceInit() {
  // 注册信号处理函数
  signal(SIGSEGV, signalhandler);
  signal(SIGABRT, signalhandler);
  signal(SIGILL, signalhandler);
}
