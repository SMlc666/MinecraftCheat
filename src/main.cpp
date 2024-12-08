#include <dobby.h>
#include <thread>
#include <jni.h>
#include "KittyMemory.hpp"
#include "imgui.h"
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"
JNIEnv *env = nullptr;
JavaVM *jvm = nullptr;
void setup() {
    
}
void inject() {

}
extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM* vm, void *key)
{
    jvm = vm;
    JNIEnv *m_env = nullptr;
    vm->GetEnv((void**)&m_env, JNI_VERSION_1_6);
    env = m_env;
    std::thread(setup).detach();
    return JNI_VERSION_1_6;
}