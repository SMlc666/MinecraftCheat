#include <dobby.h>
#include <thread>
#include <dlfcn.h>
#include <jni.h>
#include "main.hpp"
#include "KittyMemory.hpp"
JNIEnv *env = nullptr;
JavaVM *jvm = nullptr;
void setup() {
  dlopen("libEGL.so", 4);
}
extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM *vm, void *key) {
  jvm = vm;
  JNIEnv *m_env = nullptr;
  vm->GetEnv((void **)&m_env, JNI_VERSION_1_6);
  env = m_env;
  std::thread(setup).detach();
  return JNI_VERSION_1_6;
}