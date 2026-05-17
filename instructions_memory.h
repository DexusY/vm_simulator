#ifndef INSTRUCTIONS_MEMORY_H
#define INSTRUCTIONS_MEMORY_H

#include <stdint.h>
#include "vm_config.h"

void execute_set(CPU *cpu, uint8_t reg, uint8_t value);

#endif
