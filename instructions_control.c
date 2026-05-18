#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"
#include "flag.h"

void execute_halt(CPU *cpu) {
    cpu->running = 0;
    printf("[CPU] HLT\n");
}

void execute_cmp(CPU *cpu, uint8_t reg, uint8_t val) {
    uint8_t a = cpu->registers[reg];
    uint8_t b = val;
    
    uint16_t result16 = (uint16_t)a - (uint16_t)b;
    
    update_flags(cpu, result16, a, ~b);
    
    printf("[ALU] CMP: R%d(0x%02X) - 0x%02X \n", reg, a, b);
    print_flags(cpu);
}

void execute_jmp(CPU *cpu, uint8_t addr) {
    cpu->pc = addr;
    printf("[Control] JMP -> 0x%02X\n", addr);
}

void execute_jz(CPU *cpu, uint8_t addr) {   // ZF=1 (A == B)
    if (cpu->flags & FLAG_ZF) {
        cpu->pc = addr;
        printf("[Control] JZ -> 0x%02X (Skok)\n", addr);
    }
}

void execute_jnz(CPU *cpu, uint8_t addr) {  // ZF!=1 (A != B)
    if (!(cpu->flags & FLAG_ZF)) {
        cpu->pc = addr;
        printf("[Control] JNZ -> 0x%02X (Skok)\n", addr);
    }
}

void execute_jg(CPU *cpu, uint8_t addr) {   // Need to check both SF and OF for signed comparisons
    int sf = !!(cpu->flags & FLAG_SF);
    int of = !!(cpu->flags & FLAG_OF);
    int zf = !!(cpu->flags & FLAG_ZF);

    if ((sf == of) && (zf == 0)) {
        cpu->pc = addr;
        printf("[Control] JG -> 0x%02X (Skok)\n", addr);
    }
}

void execute_jl(CPU *cpu, uint8_t addr) {   // SF != OF
    int sf = !!(cpu->flags & FLAG_SF);
    int of = !!(cpu->flags & FLAG_OF);

    if (sf != of) {
        cpu->pc = addr;
        printf("[Control] JL -> 0x%02X (Skok)\n", addr);
    }
}

void execute_call(CPU *cpu, uint8_t addr) {
    cpu->sp--;
    cpu->memory[cpu->sp] = cpu->pc + 2; 
    cpu->pc = addr;
    printf("[Control] CALL -> 0x%02X\n", addr);
}

void execute_ret(CPU *cpu) {
    cpu->pc = cpu->memory[cpu->sp];
    cpu->sp++;
    printf("[Control] RET -> 0x%02X\n", cpu->pc);
}





