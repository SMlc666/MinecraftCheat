#include "log.hpp"
#include "sighandle.hpp"
#include <dlfcn.h>
#include <unwind.h>
static void signalhandler(int sig) {
  g_log_tool.SaveToFile();
  std::terminate();
}

void sighandleInit() {
  // 注册信号处理函数
  signal(SIGSEGV, signalhandler);
  signal(SIGILL, signalhandler);
}
