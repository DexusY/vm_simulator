#ifndef INSTRUCTIONS_CONTROL_H
#define INSTRUCTIONS_CONTROL_H

#include <stdint.h>
#include "vm_config.h"

void execute_halt(CPU *cpu);
void execute_cmp(CPU *cpu, uint8_t req, uint8_t val); 
void execute_jmp(CPU *cpu, uint8_t addr); 
void execute_jz(CPU *cpu, uint8_t addr); 
void execute_jnz(CPU *cpu, uint8_t addr); 
void execute_jg(CPU *cpu, uint8_t addr); 
void execute_jl(CPU *cpu, uint8_t addr); 
void execute_call(CPU *cpu, uint8_t addr); 
void execute_ret(CPU *cpu); 
    
#endif
