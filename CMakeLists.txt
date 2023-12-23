cmake_minimum_required(VERSION 3.10)

project(XMath)


add_executable(Main)
set_property(TARGET Main PROPERTY CXX_EXTENSIONS FALSE)
set_property(TARGET Main PROPERTY CXX_STANDARD 20)

target_sources(Main PRIVATE
    Main.cc
)

add_executable(Test Test.cc)
set_property(TARGET Test PROPERTY CXX_EXTENSIONS FALSE)
set_property(TARGET Test PROPERTY CXX_STANDARD 20)
target_compile_options(Test PRIVATE "-Wall" "-Wextra" "-pedantic")
