#ifndef INSTRUCTIONS_MEMORY_H
#define INSTRUCTIONS_MEMORY_H

#include <stdint.h>
#include "vm_config.h"

void execute_set(CPU *cpu, uint8_t reg, uint8_t value);
void execute_load(CPU *cpu, uint8_t reg, uint8_t addr);
void execute_store(CPU *cpu, uint8_t reg, uint8_t addr);
void execute_mov(CPU *cpu, uint8_t dst, uint8_t src);

#endif
