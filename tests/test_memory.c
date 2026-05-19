#include "test_framework.h"
#include "../vm_config.h"
#include "../instructions_memory.h"

static void test_set_writes_register(void) {
    CPU cpu = {0};
    execute_set(&cpu, 0, 42);
    ASSERT_EQ(42, cpu.registers[0]);
}

static void test_set_invalid_register_is_a_no_op(void) {
    CPU cpu = {0};
    execute_set(&cpu, NUM_REGISTERS, 99);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        ASSERT_EQ(0, cpu.registers[i]);
    }
}

static void test_load_copies_memory_to_register(void) {
    CPU cpu = {0};
    cpu.memory[0x10] = 0xAB;
    execute_load(&cpu, 1, 0x10);
    ASSERT_EQ(0xAB, cpu.registers[1]);
}

static void test_load_invalid_register_is_a_no_op(void) {
    CPU cpu = {0};
    cpu.memory[0x10] = 0xAB;
    execute_load(&cpu, NUM_REGISTERS, 0x10);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        ASSERT_EQ(0, cpu.registers[i]);
    }
}

static void test_store_copies_register_to_memory(void) {
    CPU cpu = {0};
    cpu.registers[2] = 0xCD;
    execute_store(&cpu, 2, 0x20);
    ASSERT_EQ(0xCD, cpu.memory[0x20]);
}

static void test_store_invalid_register_is_a_no_op(void) {
    CPU cpu = {0};
    execute_store(&cpu, NUM_REGISTERS, 0x20);
    ASSERT_EQ(0, cpu.memory[0x20]);
}

static void test_mov_copies_register_to_register(void) {
    CPU cpu = {0};
    cpu.registers[0] = 7;
    execute_mov(&cpu, 1, 0);
    ASSERT_EQ(7, cpu.registers[1]);
    ASSERT_EQ(7, cpu.registers[0]);
}

static void test_mov_invalid_destination_is_a_no_op(void) {
    CPU cpu = {0};
    cpu.registers[0] = 7;
    execute_mov(&cpu, NUM_REGISTERS, 0);
    ASSERT_EQ(7, cpu.registers[0]);
}

static void test_mov_invalid_source_is_a_no_op(void) {
    CPU cpu = {0};
    cpu.registers[1] = 5;
    execute_mov(&cpu, 1, NUM_REGISTERS);
    ASSERT_EQ(5, cpu.registers[1]);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_set_writes_register);
    RUN_TEST(test_set_invalid_register_is_a_no_op);
    RUN_TEST(test_load_copies_memory_to_register);
    RUN_TEST(test_load_invalid_register_is_a_no_op);
    RUN_TEST(test_store_copies_register_to_memory);
    RUN_TEST(test_store_invalid_register_is_a_no_op);
    RUN_TEST(test_mov_copies_register_to_register);
    RUN_TEST(test_mov_invalid_destination_is_a_no_op);
    RUN_TEST(test_mov_invalid_source_is_a_no_op);

    TEST_SUMMARY();
}
