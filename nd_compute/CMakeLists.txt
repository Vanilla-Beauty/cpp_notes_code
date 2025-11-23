cmake_minimum_required(VERSION 3.31)
project(nd_compute)

set(CMAKE_CXX_STANDARD 23)

add_executable(nd_compute main.cpp
        numcpp/numcpp.ixx
        numcpp/array/array.ixx
        numcpp/creation.ixx
        numcpp/algorithm.ixx
        numcpp/array/view.ixx
        numcpp/concepts.ixx
        numcpp/types.ixx)
