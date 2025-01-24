add_rules("mode.debug", "mode.release")
set_config("runtimes","c++_static")
set_config("ndk_sdkver","23")
local is_arm64_v8a = is_arch("arm64-v8a")
local is_android = is_plat("android")
local DrawDir = "src/Draw"
local LogDir = "src/Log"
local ModuleDir = "src/Module"
local MinecraftDir = "src/Minecraft"
local ScriptDir = "src/Script"
local MemToolDir = "src/MemTool"
local iMsgCaptureDir= "src/include/iMsgCapture"
local DobbyDir = "src/include/Dobby"
local LuaDir = "src/include/Lua"
local LuaSocketDir = "src/include/LuaSocket"
local EnttDir = "src/include/entt"
local KittyMemoryDir = "src/include/KittyMemory/"
local KeystoneDir = "src/include/Keystone"
local ImguiDir = "src/include/imgui"
local IncludeDir = "src/include"
if is_mode("debug") then
    set_warnings("all")
    set_optimize("none")
    set_symbols("debug")   -- 打开调试符号
else
    set_optimize("fastest")  -- 设置快速优化
    set_symbols("hidden")
    set_strip("all")
end
target("cheat")
    set_default(true)
    set_languages("c17", "cxx20")
    set_kind("shared")
    add_files("src/main.cpp")--main src file
    add_files(ScriptDir.. "/*.cpp")
    add_files(ScriptDir.."/API/**.cpp")
    add_files(DrawDir.. "/*.cpp")
    add_files(DrawDir.."/Touch/touch.cpp")
    add_files(DrawDir.."/Module/shortcut.cpp")
    add_files(LogDir.. "/*.cpp")
    add_files(LogDir.."/sighandle/*.cpp")
    add_files(iMsgCaptureDir.. "/**.cpp")
    add_files(LuaSocketDir.."/*.c")
    add_files(ModuleDir.. "/**.cpp|examples/example.cpp")
    add_files(MinecraftDir.. "/**.cpp")
    add_files(MemToolDir.. "/**.cpp")
    add_files(LuaDir.. "/*.c")
    add_files(KittyMemoryDir.."/*.cpp")
    add_files(ImguiDir.."/*.cpp")--imgui source files
    add_files(ImguiDir.."/backends/imgui_impl_android.cpp")
    add_files(ImguiDir.."/backends/imgui_impl_opengl3.cpp")
    add_linkdirs(KeystoneDir.. "/android/arm64-v8a")
    add_links("keystone")
    add_linkdirs(DobbyDir.. "/android/arm64-v8a")
    add_links("dobby")
    add_includedirs(
        "src",
        ModuleDir,
        DrawDir,
        LogDir,
        MemToolDir,
        MinecraftDir,
        ScriptDir,
        IncludeDir
    )
    add_links("android","log","EGL","GLESv3","dl")