#ifndef INSTRUCTIONS_STACK_H
#define INSTRUCTIONS_STACK_H

#include <stdint.h>
#include "vm_config.h"

void execute_psh(CPU *cpu, uint8_t value);
void execute_pop(CPU *cpu);
void execute_dup(CPU *cpu);
void execute_nop(CPU *cpu);
void execute_swap(CPU *cpu);

#endif
