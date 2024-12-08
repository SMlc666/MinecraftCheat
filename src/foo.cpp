#include <dobby.h>
#include <thread>
#include <jni.h>
#include "KittyMemory.hpp"
void setup() {
    
}
void inject() {

}
extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM* vm, void *key)
{
    // key 1337 is passed by injector
    if (key != (void*)1337)
        return JNI_VERSION_1_6;

    KITTY_LOGI("JNI_OnLoad called by injector.");

    JNIEnv *env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK)
    {
        KITTY_LOGI("JavaEnv: %p.", env);
        // ...
    }

    
    
    return JNI_VERSION_1_6;
}