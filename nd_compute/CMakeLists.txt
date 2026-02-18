cmake_minimum_required(VERSION 3.31)
project(nd_compute)

set(CMAKE_CXX_STANDARD 23)

add_executable(nd_compute main.cpp
        numcpp/numcpp.ixx
        numcpp/array/array.ixx
        numcpp/array/view.ixx
        numcpp/concepts.ixx
        numcpp/types.ixx
        numcpp/api/creation.ixx
        numcpp/algorithm/algorithm.ixx
        numcpp/algorithm/aggregate.ixx
        numcpp/utils.ixx
        numcpp/algorithm/universial.ixx
        numcpp/algorithm/basic.ixx)
