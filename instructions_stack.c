#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_psh(CPU *cpu, uint8_t value) {
    if ((uint8_t)(cpu->sp - 1) == cpu->pc) { // catches stack growing into program code area
        printf("ERROR: Stack overflow!\n");
        return;
    }
    cpu->sp--;
    cpu->memory[cpu->sp] = value;
    printf("[Stack] Pushed value %d at address 0x%02X\n", value, cpu->sp);
}

void execute_pop(CPU *cpu) {
    if (cpu->sp == 0) { // catches pop on empty stack (sp==0 is the initial empty state)
        printf("ERROR: Stack underflow!\n");
        return;
    }
    uint8_t value = cpu->memory[cpu->sp++];
    printf("[Stack] Popped value %d\n", value);
}

void execute_dup(CPU *cpu) {
    if (cpu->sp == 0) { // catches dup on empty stack, nothing to duplicate
        printf("ERROR: Stack underflow!\n");
        return;
    }
    uint8_t top = cpu->memory[cpu->sp];
    cpu->sp--;
    cpu->memory[cpu->sp] = top;
    printf("[Stack] DUP %d\n", top);
}

void execute_nop(CPU *cpu) {
    (void)cpu;  //prevent from warning
    printf("[CPU] NOP\n");
}

void execute_swap(CPU *cpu) {
    if (cpu->sp == 0 || cpu->sp == 0xFF) { // catches fewer than 2 elements (sp+1 would leave stack region)
        printf("ERROR: Not enough elements to swap!\n");
        return;
    }
    uint8_t a = cpu->memory[cpu->sp];
    uint8_t b = cpu->memory[cpu->sp + 1];
    cpu->memory[cpu->sp]     = b;
    cpu->memory[cpu->sp + 1] = a;
    printf("[Stack] SWAP %d <-> %d\n", a, b);
}