#ifndef INSTRUCTIONS_ALU_H
#define INSTRUCTIONS_ALU_H

#include <stdint.h>
#include "vm_config.h"

void execute_add(CPU *cpu);
void execute_sub(CPU *cpu);
void execute_mul(CPU *cpu);
void execute_div(CPU *cpu);
void execute_mod(CPU *cpu);
void execute_and(CPU *cpu);
void execute_or(CPU *cpu);
void execute_xor(CPU *cpu);
void execute_not(CPU *cpu);
void execute_shl(CPU *cpu);
void execute_shr(CPU *cpu);
#endif
