-- xmake.lua
set_languages("c++20")

-- 启用模块支持（关键！）
set_policy("build.c++.modules", true)

target("modules_demo")
    set_kind("binary")
    add_files("src/main.cpp")
    add_files("src/math/math.ixx")           -- 主模块接口
    add_files("src/math/math_impl.cpp")      -- 模块实现单元
    add_files("src/math/utils.ixx")     -- 模块分区