#ifndef VM_CONFIG_H
#define VM_CONFIG_H

#include <stdint.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 4

typedef enum {
    HLT = 0,
    PSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    XOR,
    NOT,
    SET,
    DUP,
    NOP,
    SWAP,
} InstructionSet;


typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t registers[NUM_REGISTERS];
    uint8_t pc; // Program Counter                                                                                                                                                                                                                                                                             
    uint8_t sp; // Stack Pointer  
    uint8_t running;
} CPU;

#endif 