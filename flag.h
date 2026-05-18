#ifndef FLAG_H
#define FLAG_H

#include "vm_config.h"

static void update_flags(CPU *cpu, uint16_t result16, uint8_t a, uint8_t b);
static void print_flags(CPU *cpu);

#endif
