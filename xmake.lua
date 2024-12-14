add_rules("mode.debug", "mode.release")
local is_arm64_v8a = is_arch("arm64-v8a")
local is_armeabi_v7a = is_arch("armeabi-v7a")
local is_x86 = is_arch("x86")
local is_x86_64 = is_arch("x86_64")
local is_android = is_plat("android")
local DrawDir = "src/Draw"
local LogDir = "src/Log"
local ModuleDir = "src/Module"
local ScriptDir = "src/Script"
local DobbyDir = "src/include/Dobby"
local LuaDir = "src/include/Lua"
local EnttDir = "src/include/entt"
local KittyMemoryDir = "src/include/KittyMemory/"
local KeystoneDir = "src/include/KittyMemory/Deps/Keystone/libs-android"
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
function add_archlinks(arch) 
    add_linkdirs(KeystoneDir.. "/".. arch)
    add_links("keystone")
    if is_plat("android") then
        add_linkdirs(DobbyDir.. "/android/".. arch)
        add_links("dobby")
    elseif is_plat("linux") then 
        add_linkdirs(DobbyDir.. "/linux/".. arch)
        add_links("dobby")
    end 
end
target("cheat")
    set_default(true)
    set_languages("c17", "cxx20")
    set_kind("shared")
    add_files("src/main.cpp")--main src file
    add_files(ScriptDir.. "/*.cpp")
    add_files(DrawDir.. "/*.cpp")
    add_files(LogDir.. "/*.cpp")
    add_files(ModuleDir.. "/**.cpp|examples/example.cpp")
    add_files(LuaDir.. "/*.c")
    add_files(KittyMemoryDir.."/*.cpp")
    add_files(ImguiDir.."/*.cpp")--imgui source files
    add_files(ImguiDir.."/backends/imgui_impl_android.cpp")
    add_files(ImguiDir.."/backends/imgui_impl_opengl3.cpp")
    add_includedirs(
        "src",
        ModuleDir,
        DrawDir,
        LogDir,
        IncludeDir
    )
    on_load(function (target)
        if target:name() == "cheat" then
            if not is_x86 and not is_x86_64 and not is_armeabi_v7a and not is_arm64_v8a then
                raise("not support this arch")
            end
        end
    end)
    add_links("android","log","EGL","GLESv3","dl")
    if is_arm64_v8a then 
        add_archlinks("arm64-v8a")
    elseif is_armeabi_v7a then 
        add_archlinks("armeabi-v7a")
    elseif is_x86_64 then 
        add_archlinks("x86_64")
    elseif is_x86 then 
        add_archlinks("x86")
    end

target("example_Script")
    set_languages("c17", "cxx20")
    set_kind("binary")
    add_files(LuaDir.. "/*.c")
    add_files(ScriptDir.. "/examples/example.cpp")
    add_includedirs(
        LuaDir,
        DobbyDir
    )
    if is_arm64_v8a then 
        add_archlinks("arm64-v8a")
    elseif is_armeabi_v7a then 
        add_archlinks("armeabi-v7a")
    elseif is_x86_64 then 
        add_archlinks("x86_64")
    elseif is_x86 then 
        add_archlinks("x86")
    end