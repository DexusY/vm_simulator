#include <stdint.h>

#include "program_loader.h"
#include "vm_config.h"
#include "cpu.h"

int main() {
    const uint8_t program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        POP,
        HLT
    };

    CPU cpu = {0};
    cpu.running = 1;

    int program_size = sizeof(program) / sizeof(program[0]);
    loader(program, program_size, cpu.memory);

    while (cpu.running) {
        cpu_step(&cpu);
    }

    return 0;
}