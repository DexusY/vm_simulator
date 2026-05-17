#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "program_loader.h"
#include "vm_config.h"
#include "instructions_alu.h"
#include "instructions_control.h"
#include "instructions_memory.h"
#include "instructions_stack.h"

int main() {
    const uint8_t program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        POP,
        HLT
    };

    CPU cpu = {0};
    cpu.running = 1;

    int program_size = sizeof(program) / sizeof(program[0]);
    loader(program, program_size, cpu.memory);

    while (cpu.running) {
        uint8_t opcode = cpu.memory[cpu.pc++];

        switch (opcode) {
            case HLT: execute_halt(&cpu);                       break;
            case PSH: execute_psh(&cpu, cpu.memory[cpu.pc++]);  break;
            case ADD: execute_add(&cpu);                        break;
            case POP: execute_pop(&cpu);                        break;
            case SET: { uint8_t reg = cpu.memory[cpu.pc++]; uint8_t val = cpu.memory[cpu.pc++]; execute_set(&cpu, reg, val); break; }
        }
    }

    return 0;
}