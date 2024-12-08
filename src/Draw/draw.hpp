#pragma once
#include <dlfcn.h>
#include "main.hpp"
#include "imgui.h"
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/native_window_jni.h>
extern int g_GlHeight, g_GlWidth;
extern ANativeWindow *g_Window;
void drawSetup();