#include <iostream>

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
                    //TODO clear screen
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
            pc += 2;
            break;
        case 0x2000: //calls subroutine at NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000: {
            unsigned short x = 0x0F00 & opcode;
            unsigned char n = 0x00FF & opcode;
            v[x] = n;
            pc += 2;
            break;
        }
        case 0x7000:
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0001:
                    break;
                case 0x0002:
                    break;
                case 0x0003:
                    break;
                case 0x0004:
                    break;
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
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            switch (opcode & 0x000F){
                case 0x0001:
                    break;
                case 0x000E:
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0015:
                    break;
                case 0x0018:
                    break;
                case 0x001E:
                    break;
                case 0x0029:
                    break;
                case 0x0033:
                    break;
                case 0x0055:
                    break;
                case 0x0065:
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;
        default:
            printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
    }

    //TODO delay timer
    //TODO sound timer
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    initialize();
    return 0;
}
