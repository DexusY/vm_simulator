#ifndef VM_CONFIG_H
#define VM_CONFIG_H

#include <stdint.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 4

typedef enum {
    HLT = 0, 
    PSH,     
    ADD,     
    POP,     
    SET,     
} InstructionSet;


typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t registers[NUM_REGISTERS];
    uint8_t pc; // Program Counter
    uint8_t sp; // Stack Pointer
} CPU;

#endif 