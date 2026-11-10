cmake_minimum_required(VERSION 3.10.0)
project(practice VERSION 0.1.0 LANGUAGES C CXX)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++20)
    add_compile_options(/EHsc)
    add_compile_options(/Od)
else()
    add_compile_options(-Wall)
    add_compile_options(-std=c++20)
    add_compile_options(-fexceptions)
    add_compile_options(-O0)
    add_compile_options(-DGCC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

add_executable(out
    main.cpp
)

# tasks.json
# args: [, ..., "-g", "{file}", "-o", ...]
#
# args: [, ..., "-g", "${fileDirname}/*.cpp", "-o", ...] - unix
# args: [, ..., "-g", "${fileDirname}\\**.cpp", "-o", ...] - win

