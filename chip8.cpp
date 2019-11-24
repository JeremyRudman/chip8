#include <iostream>
#include <unistd.h>

unsigned short opcode;
unsigned char memory[4096];
unsigned char v[16];
unsigned short I;
unsigned short pc = 0;
unsigned char gfx[64 * 32];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned short stack[16];
unsigned short sp;
unsigned char key[16];

unsigned char chip8_fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };



void initialize() {
    pc = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;      // Reset stack pointer

    delay_timer = 60;
    sound_timer = 60;

    for (unsigned char & j : gfx) { // Clear display
        j = 0;
    }

    for (unsigned short & k : stack) { // Clear stack
        k = 0;
    }

    for (unsigned char & k :v){ // Clear registers V0-VF
        k = 0;
    }

    for (unsigned char & i : memory){ // Clear memory
         i=0;
    }

    for (int i = 0; i < 80; ++i) {
        memory[i] = chip8_fontset[i];
    }
}

void loadgame(char* game){ //TODO file input
    int bufferSize;
    unsigned char buffer[0];
    for(int i = 0; i < bufferSize; ++i)
        memory[i + 512] = buffer[i];
}

void emulateCycle() {

    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    for(unsigned char & i :gfx){
                        i=0;
                    }
                    break;
                case 0x000E:
                    //TODO return from a subroutine
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        case 0x1000: //goes to NNN
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: //calls subroutine at NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: { //skip next instruction if V[X] == NN
            unsigned short x = (0x0F00 & opcode)>>8;
            unsigned char n = 0x00FF & opcode;
            if (v[x] == n) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x4000: { //skip next instruction if V[X] != NN
            unsigned short x = (0x0F00 & opcode)>>8;
            unsigned char n = 0x00FF & opcode;
            if (v[x] != n) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x5000: { //skip next instruction if V[X] != V[Y]
            unsigned short x = (0x0F00 & opcode) >> 8;
            unsigned short y = (0x00F0 & opcode) >> 4;
            if (v[x] == v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x6000: {
            unsigned short x = (0x0F00 & opcode)>>8;
            unsigned char n = 0x00FF & opcode;
            v[x] = n;
            pc += 2;
            break;
        }
        case 0x7000: {
            unsigned short x = (0x0F00 & opcode) >> 8;
            unsigned char n = 0x00FF & opcode;
            v[x] = v[x] + n;
            pc += 2;
            break;
        }
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0001: {
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    unsigned short y = (0x00F0 & opcode) >> 4;
                    v[x] = v[y];
                    pc += 2;
                    break;
                }
                case 0x0002:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    unsigned short y = (0x00F0 & opcode) >> 4;
                    v[x] = v[y] | v[x];
                    pc += 2;
                    break;
                }
                case 0x0003:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    unsigned short y = (0x00F0 & opcode) >> 4;
                    v[x] = v[y] & v[x];
                    pc += 2;
                    break;
                }
                case 0x0004:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    unsigned short y = (0x00F0 & opcode) >> 4;
                    v[x] = v[y] ^ v[x];
                    pc += 2;
                    break;
                }
                case 0x0005:
                    break;
                case 0x0006:
                    break;
                case 0x0007:
                    break;
                case 0x000E:
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        case 0x9000:{ //skip next instruction if V[X] != V[Y]
            unsigned short x = (0x0F00 & opcode) >> 8;
            unsigned short y = (0x00F0 & opcode) >> 4;
            if (v[x] != v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0xA000: {
            unsigned short n = opcode & 0x0FFF;
            I = n;
            break;
        }
        case 0xB000: {
            pc = (0x0FFF & opcode) + v[0];
            break;
        }
        case 0xC000: {
            unsigned short random = rand() % 256;
            unsigned short x = (opcode & 0x0F00) >> 8;
            unsigned short n = (opcode & 0x00FF);
            v[x] = random & n;
            break;
        }
        case 0xD000:
            break;
        case 0xE000:
            switch (opcode & 0x000F){
                case 0x0001: {
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    if(key[x]==1){
                        pc+=4;
                    }
                    else{
                        pc+=2;
                    }
                    break;
                }
                case 0x000E:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    if(key[x]!=1){
                        pc+=4;
                    }
                    else{
                        pc+=2;
                    }
                    break;
                }
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007: {
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    v[x] = delay_timer;
                    break;
                }
                case 0x000A:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    bool pressed=false;
                    while(true){
                        for (unsigned char i : key) {
                            if(i==1){
                                pressed = true;
                                break;
                            }
                        }
                        if(pressed){
                            break;
                        }
                    }
                    v[x]=1;
                    break;
                }
                case 0x0015:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    delay_timer = v[x];
                    break;
                }
                case 0x0018:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    sound_timer = v[x];
                    break;
                }
                case 0x001E:{
                    unsigned short x = (0x0F00 & opcode) >> 8;
                    if(I + v[x] > 0xFFF){
                        v[15]=1;
                    } else{
                        v[15]=0;
                    }
                    I = I + v[x];
                    break;
                }
                case 0x0029:
                    break;
                case 0x0033:
                    break;
                case 0x0055: {
                    unsigned short r = (0x0F00 * opcode) >> 8;
                    for (int i = 0; i <= r; ++i) {
                        memory[I+i]=v[i];
                    }
                    break;
                }
                case 0x0065: {
                    unsigned short r = (0x0F00 * opcode) >> 8;
                    for (int i = 0; i <= r; ++i) {
                        v[i] = memory[I+i];
                    }
                    break;
                }
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        default:
            printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    initialize();
    return 0;
}
