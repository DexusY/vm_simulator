#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "program_loader.h"
#include "vm_config.h"

int main() {
    const uint8_t program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        POP,
        HLT
    };

    CPU cpu = {0};

    int program_size = sizeof(program) / sizeof(program[0]);
    
    loader(program, program_size, cpu.memory);
    uint8_t fetch = cpu.memory[cpu.pc];

    return 0;
}