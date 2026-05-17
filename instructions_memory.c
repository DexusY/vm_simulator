#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_set(CPU *cpu, uint8_t reg, uint8_t value) {
    if (reg >= NUM_REGISTERS) { // catches out-of-range register index (only R0–R3 exist)
        printf("ERROR: Invalid register R%d!\n", reg);
        return;
    }
    cpu->registers[reg] = value;
    printf("[Registers] SET R%d = %d\n", reg, value);
}

void execute_load(CPU *cpu, uint8_t reg, uint8_t addr) {
    if (reg >= NUM_REGISTERS) { // catches destination register out of range before writing into it
        printf("ERROR: Invalid register R%d!\n", reg);
        return;
    }
    cpu->registers[reg] = cpu->memory[addr];
    printf("[Memory] LOAD R%d = memory[0x%02X] (value: 0x%02X)\n", reg, addr, cpu->registers[reg]);
}

void execute_store(CPU *cpu, uint8_t reg, uint8_t addr) {
    if (reg >= NUM_REGISTERS) { // catches source register out of range before reading from it
        printf("ERROR: Invalid register R%d!\n", reg);
        return;
    }
    cpu->memory[addr] = cpu->registers[reg];
    printf("[Memory] STORE memory[0x%02X] = R%d (%d)\n", addr, reg, cpu->registers[reg]);
}

void execute_mov(CPU *cpu, uint8_t dst, uint8_t src) {
    if (dst >= NUM_REGISTERS || src >= NUM_REGISTERS) { // catches either register being out of range
        printf("ERROR: Invalid register!\n");
        return;
    }
    cpu->registers[dst] = cpu->registers[src];
    printf("[Memory] MOV R%d = R%d (%d)\n", dst, src, cpu->registers[dst]);
}