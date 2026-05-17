#ifndef INSTRUCTIONS_SET_H
#define INSTRUCTIONS_SET_H

#include <stdint.h>
#include "vm_config.h"

void execute_psh(CPU *cpu, uint8_t value);
void execute_pop(CPU *cpu);
void execute_add(CPU *cpu);
void execute_set(CPU *cpu, uint8_t reg, uint8_t value);
void execute_halt(CPU *cpu);

#endif
