#include "test_framework.h"
#include "../vm_config.h"
#include "../cpu.h"
#include "../program_loader.h"

static void run_program(CPU *cpu, const uint8_t *program, int size) {
    cpu->running = 1;
    loader(program, size, cpu->memory);
    while (cpu->running) {
        cpu_step(cpu);
    }
}

static void test_program_add_and_halt(void) {
    CPU cpu = {0};
    const uint8_t program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        HLT,
    };
    run_program(&cpu, program, (int)sizeof(program));
    ASSERT_EQ(11, cpu.memory[cpu.sp]);
    ASSERT_EQ(0, cpu.running);
}

static void test_program_set_then_store(void) {
    CPU cpu = {0};
    const uint8_t program[] = {
        SET,   1, 0x77,
        STORE, 1, 0x40,
        HLT,
    };
    run_program(&cpu, program, (int)sizeof(program));
    ASSERT_EQ(0x77, cpu.registers[1]);
    ASSERT_EQ(0x77, cpu.memory[0x40]);
}

static void test_program_conditional_jump_taken(void) {
    CPU cpu = {0};
    const uint8_t program[] = {
        /* 0  */ SET, 0, 5,
        /* 3  */ CMP, 0, 5,
        /* 6  */ JZ, 11,
        /* 8  */ SET, 1, 0xAA,
        /* 11 */ HLT,
    };
    run_program(&cpu, program, (int)sizeof(program));
    ASSERT_EQ(0, cpu.registers[1]);
}

static void test_program_conditional_jump_not_taken(void) {
    CPU cpu = {0};
    const uint8_t program[] = {
        /* 0  */ SET, 0, 5,
        /* 3  */ CMP, 0, 9,
        /* 6  */ JZ, 11,
        /* 8  */ SET, 1, 0xAA,
        /* 11 */ HLT,
    };
    run_program(&cpu, program, (int)sizeof(program));
    ASSERT_EQ(0xAA, cpu.registers[1]);
}

static void test_program_call_and_ret(void) {
    CPU cpu = {0};
    const uint8_t program[] = {
        /* 0 */ CALL, 5,
        /* 2 */ HLT,
        /* 3 */ 0, 0,
        /* 5 */ SET, 2, 0x42,
        /* 8 */ RET,
    };
    run_program(&cpu, program, (int)sizeof(program));
    ASSERT_EQ(0x42, cpu.registers[2]);
    ASSERT_EQ(0, cpu.running);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_program_add_and_halt);
    RUN_TEST(test_program_set_then_store);
    RUN_TEST(test_program_conditional_jump_taken);
    RUN_TEST(test_program_conditional_jump_not_taken);
    RUN_TEST(test_program_call_and_ret);

    TEST_SUMMARY();
}
