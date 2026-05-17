#include <stdio.h>
#include <stdint.h>

#include "program_loader.h"

#define MEMORY_SIZE 256
#define NUM_REGISTERS 4

#define MEMORY[MEMORY_SIZE]
#define REGISTERS[NUM_REGISTERS]
#define PC

typedef enum {
    HLT = 0, 
    PSH,     
    ADD,     
    POP,     
    SET,     
} InstructionSet;

int main() {
    const uint8_t program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        POP,
        HLT
    };

    uint8_t memory[MEMORY_SIZE] = {0};
    uint8_t registers[NUM_REGISTERS] = {0};
    uint8_t pc = 0;

    int program_size = sizeof(program) / sizeof(program[0]);
    loader(program, program_size, memory);

    uint8_t fetch = memory[pc];

    return 0;

}