set_project("project-learn")
set_version("1.0.0")
set_languages("c++20")

add_requires("gtest")
add_requires("yalantinglibs")

target("struct_pack_test")
    set_kind("binary")
    add_files("test_struct_pack.cpp")
    add_packages("gtest", "yalantinglibs")

target("struct_pb_test")
    set_kind("binary")
    add_files("test_struct_pb.cpp")
    add_packages("gtest", "yalantinglibs")