#pragma once
#include <dlfcn.h>
#include "main.hpp"
#include "imgui.h"
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
extern int g_GlHeight, g_GlWidth;
void drawSetup();