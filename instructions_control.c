#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_halt(CPU *cpu) {
    cpu->running = 0;
    printf("[CPU] HLT\n");
}