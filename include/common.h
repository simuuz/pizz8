#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <ctime>
#include <chrono>
#include <SDL2/SDL.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

struct color {
    u8 r,g,b,a;
    
    auto operator=(const u32& other) {
        this->r = (other >> 24) & 0xff;
        this->g = (other >> 16) & 0xff;
        this->b = (other >> 8) & 0xff;
        this->a = other & 0xff;
    }

    auto operator==(const u32& other) -> bool {
        return (this->r == ((other >> 24) & 0xff) && this->g == ((other >> 16) & 0xff) &&
                this->b == ((other >> 8) & 0xff) && this->a == (other & 0xff));
    }
};