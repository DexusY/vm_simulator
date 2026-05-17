#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_psh(CPU *cpu, uint8_t value) {
    cpu->sp--; 
    cpu->memory[cpu->sp] = value;
    printf("[Stack] Pushed value %d at address %d\n", value, cpu->sp);
}

void execute_pop(CPU *cpu) {
    uint8_t value = cpu->memory[cpu->sp++];
    printf("[Stack] Popped value %d\n", value);
}

void execute_add(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a + b;
    cpu->memory[--cpu->sp] = result;
    printf("[Stack] ADD %d + %d = %d\n", a, b, result);
}

void execute_set(CPU *cpu, uint8_t reg, uint8_t value) {
    cpu->registers[reg] = value;
    printf("[Registers] SET R%d = %d\n", reg, value);
}

void execute_halt(CPU *cpu) {
    cpu->running = 0;
    printf("[CPU] HLT\n");
}