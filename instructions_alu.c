#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

void execute_add(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a + b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] ADD %d + %d = %d\n", a, b, result);
}

void execute_sub(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a - b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] SUB %d - %d = %d\n", a, b, result);
}

void execute_mul(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a * b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] MUL %d * %d = %d\n", a, b, result);
}

void execute_div(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a / b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] DIV %d / %d = %d\n", a, b, result);
}

void execute_mod(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a % b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] MOD %d %% %d = %d\n", a, b, result);
}

void execute_and(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a & b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] AND %d & %d = %d\n", a, b, result);
}

void execute_or(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a | b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] OR %d | %d = %d\n", a, b, result);
}

void execute_xor(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a ^ b;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] XOR %d ^ %d = %d\n", a, b, result);
}

void execute_not(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t result = ~a;
    cpu->memory[--cpu->sp] = result;
    printf("[ALU] NOT ~%d = %d\n", a, result);
}