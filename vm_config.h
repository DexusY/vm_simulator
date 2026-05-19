#ifndef VM_CONFIG_H
#define VM_CONFIG_H

#include <stdint.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 4

#define FLAG_ZF (1 << 0)  // zero flag     — result is 0
#define FLAG_CF (1 << 1)  // carry flag    — unsigned overflow (result exceeded 0xFF)
#define FLAG_SF (1 << 2)  // sign flag     — MSB of result is set (would be negative in signed)
#define FLAG_OF (1 << 3)  // overflow flag — signed overflow (sign of result is wrong)

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
    LOAD,
    STORE,
    MOV,
    SHL,
    SHR,
    DUP,
    NOP,
    SWAP,
    CMP,
    JMP,
    JZ,
    JNZ,
    JG,
    JL,
    CALL,
    RET,
} InstructionSet;


typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t registers[NUM_REGISTERS];
    uint8_t pc;      // Program Counter
    uint8_t sp;      // Stack Pointer
    uint8_t flags;   // Status flags: ZF, CF, SF, OF
    uint8_t running;
} CPU;

#endif 