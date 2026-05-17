#include <stdint.h>

void loader(const uint8_t *program, int program_size, uint8_t *memory) {
    for (int i = 0; i < program_size; i++) {
        memory[i] = program[i];
    }
}