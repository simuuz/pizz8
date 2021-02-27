cmake_minimum_required(VERSION 3.1.0)

project(chip-8)

set(CMAKE_CXX_STANDARD 17)
set(CORE src/core/)
set(INCLUDE_CORE include/core/)
set(XBYAK include/xbyak/xbyak/)

include_directories(include/)
include_directories(${INCLUDE_CORE})
include_directories(${XBYAK})
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(chip-8
    src/main.cpp
    ${CORE}emu.cpp
)

target_link_libraries(chip-8 SDL2)