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