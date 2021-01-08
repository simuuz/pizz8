#include "mem.h"

Mem::Mem(char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(-1);
    }

    std::fill(std::begin(display), std::end(display), 0x006b38ff);

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
    std::fill(std::begin(display), std::end(display), 0x006b38ff);
    std::fill(std::begin(key), std::end(key), 0);
    std::fill(std::begin(stack), std::end(stack), 0);
}