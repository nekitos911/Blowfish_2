cmake_minimum_required(VERSION 3.8)
project(Blowfish_2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        blowfish.cpp
        blowfish.h
        blowfish1.h
        constants.h
        main.cpp)

add_executable(Blowfish_2 ${SOURCE_FILES})