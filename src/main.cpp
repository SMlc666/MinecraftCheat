#include <chrono>
#include <format>
#include <thread>
#include <jni.h>
#include <filesystem>
#include "main.hpp"
#include "ShadowHook/include/shadowhook.h"
#include "Touch/touch.hpp"
#include "draw.hpp"
#include "ModuleManager.hpp"
#include "ScriptManager.hpp"
#include "config/config.hpp"
#include "sighandle/sighandle.hpp"
#include "log.hpp"
#include "signature.hpp"
#include "hooks/hooks.hpp"
#define KittyInjector
JNIEnv *g_env = nullptr;
JavaVM *g_jvm = nullptr;

void setup() {
  std::this_thread::sleep_for(std::chrono::seconds(3)); //for debug
  std::filesystem::path path(CheatDir);
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directory(path);
  } //初始化目录
#ifdef _DEBUG_
  shadowhook_init(SHADOWHOOK_MODE_SHARED, true);
#else
  shadowhook_init(SHADOWHOOK_MODE_SHARED, false);
#endif
  sighandleInit();
  signaturesInit();
  hooksInit();
  configSetup();
  drawSetup();
  touchSetup();
  moduleSetup();
  ScriptSetup();
}

extern "C" auto JNIEXPORT JNI_OnLoad(JavaVM *vm, void *reserved) -> jint { //NOLINT
  if (g_jvm != nullptr) {
    return JNI_VERSION_1_6;
  }
  g_jvm = vm;
  JNIEnv *m_env = nullptr;
  g_log_tool.message(LogLevel::INFO, "OnLoad",
                     std::format("vm: {:p}", reinterpret_cast<void *>(vm)));
  //NOLINTNEXTLINE
  vm->GetEnv(reinterpret_cast<void **>(&m_env), JNI_VERSION_1_6);
  g_env = m_env;
  std::thread(setup).detach();
  return JNI_VERSION_1_6;
}

#ifndef KittyInjector
const static int MAX_JAVAVM_BUFFER = 1;
__attribute__((constructor)) void OnLoad() {
  std::array<JavaVM *, MAX_JAVAVM_BUFFER> jvmBuffer{};
  jsize nVms = 0;
  jint status = JNI_GetCreatedJavaVMs(jvmBuffer.data(), MAX_JAVAVM_BUFFER, &nVms);
  if (status != JNI_OK || nVms == 0 || jvmBuffer[0] == nullptr) {
    g_log_tool.message(LogLevel::INFO, "OnLoad", "OnLoad called");
    return;
  }
  g_log_tool.message(LogLevel::INFO, "OnLoad", std::format("nVms: {}", nVms));
  g_jvm = jvmBuffer[0];
  JNIEnv *m_env = nullptr;
  g_log_tool.message(LogLevel::INFO, "OnLoad",
                     std::format("vm: {:p}", reinterpret_cast<void *>(g_jvm)));
  g_jvm->GetEnv(reinterpret_cast<void **>(&m_env), JNI_VERSION_1_6);
  g_env = m_env;
  std::thread(setup).detach();
}
#endif