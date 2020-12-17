cmake_minimum_required(VERSION 3.1.0)

project(chip-8)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(SRC src/)
set(SRC_CORE ${SRC}core/)
set(INCLUDE include/)
set(INCLUDE_CORE ${INCLUDE}core/)

include_directories(${INCLUDE})
include_directories(${INCLUDE_CORE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses -lSDL2 -std=c++17 -O3")

add_executable(chip-8
    ${SRC}main.cpp
    ${SRC_CORE}cpu.cpp
    ${SRC_CORE}mem.cpp)