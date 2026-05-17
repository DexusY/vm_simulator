#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <stdint.h>

void loader(const uint8_t *program, int program_size, uint8_t *memory);

#endif