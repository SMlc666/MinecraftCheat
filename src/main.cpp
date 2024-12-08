#include <dobby.h>
#include <thread>
#include <dlfcn.h>
#include <jni.h>
#include "main.hpp"
#include "draw.hpp"
#include "ModuleManager.hpp"
#include "KittyMemory.hpp"
JNIEnv *g_env = nullptr;
JavaVM *g_jvm = nullptr;
void setup() {
  drawSetup();
  moduleSetup();
}
extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM *vm, void *key) {
  g_jvm = vm;
  JNIEnv *m_env = nullptr;
  vm->GetEnv((void **)&m_env, JNI_VERSION_1_6);
  g_env = m_env;
  std::thread(setup).detach();
  return JNI_VERSION_1_6;
}