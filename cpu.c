#include "cpu.h"
#include "vm_config.h"
#include "instructions_alu.h"
#include "instructions_control.h"
#include "instructions_memory.h"
#include "instructions_stack.h"

void cpu_step(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->pc++];

    switch (opcode) {
        case PSH: execute_psh(cpu, cpu->memory[cpu->pc++]);     break;
        case POP: execute_pop(cpu);                             break;

        case ADD: execute_add(cpu);                             break;
        case SUB: execute_sub(cpu);                             break;
        case MUL: execute_mul(cpu);                             break;
        case DIV: execute_div(cpu);                             break;
        case MOD: execute_mod(cpu);                             break;
        case AND: execute_and(cpu);                             break;
        case OR: execute_or(cpu);                               break;
        case XOR: execute_xor(cpu);                             break;
        case NOT: execute_not(cpu);                             break;
        case SHL: execute_shl(cpu);                             break;
        case SHR: execute_shr(cpu);                             break;
        
        case SET:   { uint8_t reg = cpu->memory[cpu->pc++]; uint8_t val = cpu->memory[cpu->pc++]; execute_set(cpu, reg, val); break; }
        case LOAD:  { uint8_t reg = cpu->memory[cpu->pc++]; uint8_t addr = cpu->memory[cpu->pc++]; execute_load(cpu, reg, addr);   break; }
        case STORE: { uint8_t reg = cpu->memory[cpu->pc++]; uint8_t addr = cpu->memory[cpu->pc++]; execute_store(cpu, reg, addr); break; }
        case MOV:   { uint8_t dst = cpu->memory[cpu->pc++]; uint8_t src  = cpu->memory[cpu->pc++]; execute_mov(cpu, dst, src);    break; }

        case DUP:  execute_dup(cpu);                            break;
        case NOP:  execute_nop(cpu);                            break;
        case SWAP: execute_swap(cpu);                           break;

        case HLT: execute_halt(cpu);                            break;
    }
}
