#pragma once
#include "common.h"
#include "mem.h"

class Cpu {
public:
    Cpu(Mem* m_mem) : mem(m_mem) {}
    void execute(SDL_Event* event);
    void input(SDL_Event* event, bool* quit);
    Mem* mem;
    bool draw = false;
private:
    void dxyn(u8 x, u8 y, u8 n);
    u16 pc = 0x200, I = 0;
    u8 v[16] = {}, sp = 0, delay = 0, sound = 0;
};