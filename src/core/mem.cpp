#include "mem.h"

Mem::Mem(char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(-1);
    }

    for(auto& pixel : display) {
        pixel = 0x006b38ff;
    }

    int i = 0;
    char current;
    
    while(!file.eof()) {
        file.get(current);
        mem[0x200+i] = current;
        i++;
    }

    for(i = 0x50; i < 0xA0; i++) {
        mem[i] = font[i - 0x50];
    }
}

void Mem::reset() {int i = 0;
    for(auto& pixel : display) {
        pixel = 0x006b38ff;
    }

    for(auto& k : key) {
        k = 0;
    }

    for(auto& s : stack) {
        s = 0;
    }
}