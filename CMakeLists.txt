cmake_minimum_required(VERSION 3.9)

project(ifm_O3X100_driver VERSION 0.0.1 DESCRIPTION "Driver for acquiring images on an ifm O3X100 camera" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( include )

add_executable(simpletest src/simpletest.cpp src/O3X100.cpp)
