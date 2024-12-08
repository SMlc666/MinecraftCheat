add_rules("mode.debug", "mode.release")
local is_arm64_v8a = is_arch("arm64-v8a")
local is_armeabi_v7a = is_arch("armeabi-v7a")
local is_x86 = is_arch("x86")
local is_x86_64 = is_arch("x86_64")
local is_android = is_plat("android")
target("cheat")
    if is_mode("debug") then
        set_warnings("all")
        set_optimize("none")
        set_symbols("debug")   -- 打开调试符号
    else
        set_optimize("fastest")  -- 设置快速优化
        set_symbols("hidden")
        set_strip("all")
    end
    local DrawDir = "src/Draw"
    local LogDir = "src/Log"
    local DobbyDir = "src/include/Dobby"
    local LuaDir = "src/include/Lua"
    local KittyMemoryDir = "src/include/KittyMemory/"
    local KeystoneDir = "src/include/KittyMemory/Deps/Keystone/libs-android"
    local ImguiDir = "src/include/imgui"
    function add_archlinks(arch) 
        add_linkdirs(KeystoneDir.. "/".. arch)
        add_links("keystone")
        add_linkdirs(DobbyDir.. "/".. arch)
        add_links("dobby")
    end
    set_kind("shared")
    add_files("src/main.cpp")--main src file
    add_files(DrawDir.. "/*.cpp")
    add_files(LogDir.. "/*.cpp")
    add_files(LuaDir.. "/*.c")

    add_defines("__ANDROID__")
    add_files(KittyMemoryDir.."/*.cpp")

    add_files(ImguiDir.."/*.cpp")--imgui source files
    add_files(ImguiDir.."/backends/imgui_impl_android.cpp")
    add_files(ImguiDir.."/backends/imgui_impl_opengl3.cpp")
    add_includedirs(
        "src",
        DrawDir,
        LogDir,
        LuaDir,
        DobbyDir,
        KittyMemoryDir,
        ImguiDir
    )
    on_load(function (target)
        if not is_x86 and not is_x86_64 and not is_armeabi_v7a and not is_arm64_v8a then
            raise("not support this arch")
        end
        if not is_android then
            raise("not support this platform")
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