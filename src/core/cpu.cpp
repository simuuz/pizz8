#include "cpu.h"

void Cpu::execute(SDL_Event* evt) {
    u16 opcode = (mem->mem[pc] << 8) | (mem->mem[pc + 1]);
    u16 nnn = opcode & 0x0fff;
    u8  n = opcode & 0x000f;
    u8  x = (opcode & 0x0f00) >> 8;
    u8  y = (opcode & 0x00f0) >> 4;
    u8  kk = opcode & 0x00ff;
    pc += 2;
    switch(opcode & 0xf000) {
        case 0x0000:
        if (kk == 0xe0) {
            for(int yy = 0; yy < 32; yy++) {
                for(int xx = 0; xx < 64; xx++) {
                    mem->display[xx+64*yy] = 0;
                }
            }
        } else if (kk == 0xee) {
            sp--;
            pc = mem->stack[sp];
        } else {
            fprintf(stderr, "Unimplemented instruction 00%x\n", kk);
            exit(-1);
        }
        break;
        case 0x1000: pc = nnn; break;
        case 0x2000: mem->stack[sp] = pc; sp++; pc = nnn; break;
        case 0x3000: pc += (v[x] == kk) ? 2 : 0; break;
        case 0x4000: pc += (v[x] != kk) ? 2 : 0; break;
        case 0x5000: pc += (v[x] == v[y]) ? 2 : 0; break;
        case 0x6000: v[x] = kk; break;
        case 0x7000: v[x] += kk; break;
        case 0x8000:
        switch(n) {
            case 0x0: v[x] = v[y]; break;
            case 0x1: v[x] |= v[y]; break;
            case 0x2: v[x] &= v[y]; break;
            case 0x3: v[x] ^= v[y]; break;
            case 0x4: {
                u16 result = v[x] + v[y];
                if(result > 0xff) { v[0xf] = 1; } else { v[0xf] = 0; }
                v[x] = result & 0xff;
            }
            break;
            case 0x5: v[0xf] = (v[x] > v[y]) ? 1 : 0; v[x] -= v[y]; break;
            case 0x6: v[0xf] = ((v[x] & 1) == 1) ? 1 : 0; v[x] *= 0.5; break;
            case 0x7: v[0xf] = (v[y] > v[x]) ? 1 : 0; v[x] = v[y] - v[x]; break;
            case 0xe: v[0xf] = ((v[x] & 1) == 0x80) ? 1 : 0; v[x] *= 2; break;
            default:
            fprintf(stderr, "Unimplemented instruction 8XY%x\n", n);
            exit(-1);
            break;
        }
        break;
        case 0x9000: pc += (v[x] != v[y]) ? 2 : 0; break;
        case 0xa000: I = nnn; break;
        case 0xb000: pc = v[0] + nnn; break;
        case 0xc000: v[x] = (rand() % 255) & kk; break;
        case 0xd000: dxyn(x,y,n); break;
        case 0xe000:
        if(kk == 0x9e) { pc += (mem->key[v[x] % 15] == 1) ? 2 : 0; }
        else if (kk == 0xa1) { pc += (mem->key[v[x] % 15] == 0) ? 2 : 0; }
        else { fprintf(stderr, "Unimplemented instruction EX%x\n", kk); exit(-1); }
        
        break;
        case 0xf000:
        switch(kk) {
            case 0x07: v[x] = delay; break;
            case 0x0a:
            for(int i = 0; i < 16; i++) {
                if(SDL_GetKeyboardState(NULL)[mem->key[i]]) {
                    v[x] = i;
                    break;
                }
            }
            break;
            case 0x15: delay = v[x]; break;
            case 0x18: sound = v[x]; break;
            case 0x1e: I += v[x]; break;
            case 0x29: I = mem->font[v[x] & 0xf]; break;
            case 0x33:
            mem->mem[I + 2] = v[x] % 10;
            mem->mem[I + 1] = v[x] / 10 % 10;
            mem->mem[I] = v[x] / 100 % 10;
            break;
            case 0x55:
            for(int i = 0; i <= x; i++) {
                mem->mem[I + i] = v[i];
            }
            break;
            case 0x65:
            for(int i = 0; i <= x; i++) {
                v[i] = mem->mem[I + i];
            }
            break;
            default:
            fprintf(stderr, "Unimplemented instruction FX%x\n", kk);
            exit(-1);
            break;
        }
        break;
        default: fprintf(stderr,"Unimplemented instruction %x\n", opcode); exit(-1); break;
    }
    //printf("Opcode %x\n", opcode);
}

void Cpu::dxyn(u8 x, u8 y, u8 n) {
    v[0xf] = 0;
    for(int yy = 0; yy < n; yy++) {
        for(int xx = 0; xx < 8; xx++) {
            u8 data = mem->mem[I+yy];
            if(data & (0x80 >> xx)) {
                u8 cx = (v[x] + xx) % 64, cy = (v[y] + yy) % 32;
                if((mem->display[cx+64*cy] == 0xffffffff)) {
                    v[0xf] = 1;
                    mem->display[cx+64*cy] = 0x000000ff;
                } else {
                    mem->display[cx+64*cy] = 0xffffffff;
                }
                draw = true;
            }
        }
    }
}

void Cpu::input(SDL_Event* evt, bool* quit) {
    while(SDL_PollEvent(evt)) {
        switch(evt->type) {
            case SDL_QUIT: *quit = true; break;
            case SDL_KEYDOWN:
            switch(evt->key.keysym.sym) {
                case SDLK_1: mem->key[0]  = 1; break;
                case SDLK_2: mem->key[1]  = 1; break;
                case SDLK_3: mem->key[2]  = 1; break;
                case SDLK_4: mem->key[3]  = 1; break;
                case SDLK_q: mem->key[4]  = 1; break;
                case SDLK_w: mem->key[5]  = 1; break;
                case SDLK_e: mem->key[6]  = 1; break;
                case SDLK_r: mem->key[7]  = 1; break;
                case SDLK_a: mem->key[8]  = 1; break;
                case SDLK_s: mem->key[9]  = 1; break;
                case SDLK_d: mem->key[10] = 1; break;
                case SDLK_f: mem->key[11] = 1; break;
                case SDLK_z: mem->key[12] = 1; break;
                case SDLK_x: mem->key[13] = 1; break;
                case SDLK_c: mem->key[14] = 1; break;
                case SDLK_v: mem->key[15] = 1; break;
            }
            break;
            case SDL_KEYUP:
            switch(evt->key.keysym.sym) {
                case SDLK_1: mem->key[0]  = 0; break;
                case SDLK_2: mem->key[1]  = 0; break;
                case SDLK_3: mem->key[2]  = 0; break;
                case SDLK_4: mem->key[3]  = 0; break;
                case SDLK_q: mem->key[4]  = 0; break;
                case SDLK_w: mem->key[5]  = 0; break;
                case SDLK_e: mem->key[6]  = 0; break;
                case SDLK_r: mem->key[7]  = 0; break;
                case SDLK_a: mem->key[8]  = 0; break;
                case SDLK_s: mem->key[9]  = 0; break;
                case SDLK_d: mem->key[10] = 0; break;
                case SDLK_f: mem->key[11] = 0; break;
                case SDLK_z: mem->key[12] = 0; break;
                case SDLK_x: mem->key[13] = 0; break;
                case SDLK_c: mem->key[14] = 0; break;
                case SDLK_v: mem->key[15] = 0; break;
            }
            break;
        }
    }
}