cmake_minimum_required(VERSION 3.8)
project(bf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp blowfish.cpp blowfish.h constants.h blowfish1.cpp blowfish1.h)
add_executable(bf ${SOURCE_FILES})