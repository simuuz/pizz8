#include "mem.h"

Mem::Mem(char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(-1);
    }
    int i = 0;
    char current;
    for(int i = 0; i < 64*32*4; i++) { display[i] = 0; }
    while(!file.eof()) {
        file.get(current);
        mem[0x200+i] = current;
        i++;
    }

    for(i = 0x50; i < 0xA0; i++) {
        mem[i] = font[i - 0x50];
    }
}