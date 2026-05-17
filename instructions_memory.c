#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_set(CPU *cpu, uint8_t reg, uint8_t value) {
    cpu->registers[reg] = value;
    printf("[Registers] SET R%d = %d\n", reg, value);
}

