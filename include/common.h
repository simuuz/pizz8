#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <ctime>
#include <chrono>
#include <SDL2/SDL.h>
#include <array>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

struct Color {
    u8 r, g, b, a;

    auto operator=(const u32& other) {
        r = (other >> 24) & 0xff;
        g = (other >> 16) & 0xff;
        b = (other >> 8) & 0xff;
        a = other & 0xff;
    }

    auto operator==(const u32& other) -> bool {
        return (r == ((other >> 24) & 0xff) && g == ((other >> 16) & 0xff) &&
                b == ((other >> 8) & 0xff) && a == (other & 0xff));
    }
};
