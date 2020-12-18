#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <ctime>
#include <SDL2/SDL.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

struct color {
    u8 r,g,b,a;
};