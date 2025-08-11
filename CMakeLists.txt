cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

project(Main)

add_executable(Main main.cpp)


include_directories(Main PUBLIC include/)
