cmake_minimum_required(VERSION 3.22)
project(asyncfs)

set(CMAKE_CXX_STANDARD 23)

add_executable(asyncfs main.cpp
        src/core/concepts.ixx
        src/core/core.ixx
        src/task/asyncTaskQueue.ixx
        src/task/asyncTaskLoop.ixx
        src/task/coroutine.ixx
        src/task/asyncify.ixx)
