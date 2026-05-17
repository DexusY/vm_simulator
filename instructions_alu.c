#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

// computes and stores ZF, CF, SF, OF based on a 16-bit result and the original operands
static void update_flags(CPU *cpu, uint16_t result16, uint8_t a, uint8_t b) {
    uint8_t result = (uint8_t)result16;
    cpu->flags = 0;
    if (result == 0)       cpu->flags |= FLAG_ZF;
    if (result16 > 0xFF)   cpu->flags |= FLAG_CF;
    if (result & 0x80)     cpu->flags |= FLAG_SF;
    // signed overflow: operands share a sign but result has a different sign
    if ((~(a ^ b) & (a ^ result)) & 0x80) cpu->flags |= FLAG_OF;
}

static void print_flags(CPU *cpu) {
    printf("[Flags] ZF=%d CF=%d SF=%d OF=%d\n",
        !!(cpu->flags & FLAG_ZF),
        !!(cpu->flags & FLAG_CF),
        !!(cpu->flags & FLAG_SF),
        !!(cpu->flags & FLAG_OF));
}

void execute_add(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint16_t result16 = (uint16_t)a + b;
    cpu->memory[--cpu->sp] = (uint8_t)result16;
    update_flags(cpu, result16, a, b);
    printf("[ALU] ADD %d + %d = %d\n", a, b, (uint8_t)result16);
    print_flags(cpu);
}

void execute_sub(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint16_t result16 = (uint16_t)a - b;
    cpu->memory[--cpu->sp] = (uint8_t)result16;
    update_flags(cpu, result16, a, b);
    printf("[ALU] SUB %d - %d = %d\n", a, b, (uint8_t)result16);
    print_flags(cpu);
}

void execute_mul(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint16_t result16 = (uint16_t)a * b;
    cpu->memory[--cpu->sp] = (uint8_t)result16;
    update_flags(cpu, result16, a, b);
    printf("[ALU] MUL %d * %d = %d\n", a, b, (uint8_t)result16);
    print_flags(cpu);
}

void execute_div(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    if (b == 0) { // catches division by zero before it causes undefined behavior
        printf("ERROR: Division by zero!\n");
        return;
    }
    uint8_t result = a / b;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, b);
    printf("[ALU] DIV %d / %d = %d\n", a, b, result);
    print_flags(cpu);
}

void execute_mod(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    if (b == 0) { // catches modulo by zero before it causes undefined behavior
        printf("ERROR: Modulo by zero!\n");
        return;
    }
    uint8_t result = a % b;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, b);
    printf("[ALU] MOD %d %% %d = %d\n", a, b, result);
    print_flags(cpu);
}

void execute_and(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a & b;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, b);
    printf("[ALU] AND %d & %d = %d\n", a, b, result);
    print_flags(cpu);
}

void execute_or(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a | b;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, b);
    printf("[ALU] OR %d | %d = %d\n", a, b, result);
    print_flags(cpu);
}

void execute_xor(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t b = cpu->memory[cpu->sp++];
    uint8_t result = a ^ b;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, b);
    printf("[ALU] XOR %d ^ %d = %d\n", a, b, result);
    print_flags(cpu);
}

void execute_not(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t result = ~a;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, 0);
    printf("[ALU] NOT ~%d = %d\n", a, result);
    print_flags(cpu);
}

void execute_shl(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t n = cpu->memory[cpu->sp++];
    if (n >= 8) { // catches shift amount that would push all bits out of range
        printf("ERROR: Shift amount %d out of range (0-7)!\n", n);
        return;
    }
    uint16_t result16 = (uint16_t)a << n;  // uint16 naturally exposes bits shifted past 0xFF into CF
    cpu->memory[--cpu->sp] = (uint8_t)result16;
    update_flags(cpu, result16, a, n);
    printf("[ALU] SHL %d << %d = %d\n", a, n, (uint8_t)result16);
    print_flags(cpu);
}

void execute_shr(CPU *cpu) {
    uint8_t a = cpu->memory[cpu->sp++];
    uint8_t n = cpu->memory[cpu->sp++];
    if (n >= 8) { // catches shift amount that would push all bits out of range
        printf("ERROR: Shift amount %d out of range (0-7)!\n", n);
        return;
    }
    uint8_t result = a >> n;
    cpu->memory[--cpu->sp] = result;
    update_flags(cpu, result, a, n);
    // CF: last bit shifted out sits at position (n-1) of the original value
    if (n > 0 && (a >> (n - 1)) & 1) cpu->flags |= FLAG_CF;
    printf("[ALU] SHR %d >> %d = %d\n", a, n, result);
    print_flags(cpu);
}
