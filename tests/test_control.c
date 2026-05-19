#include "test_framework.h"
#include "../vm_config.h"
#include "../instructions_control.h"

static void test_halt_clears_running_flag(void) {
    CPU cpu = {0};
    cpu.running = 1;
    execute_halt(&cpu);
    ASSERT_EQ(0, cpu.running);
}

static void test_cmp_equal_values_set_zero_flag(void) {
    CPU cpu = {0};
    cpu.registers[0] = 5;
    execute_cmp(&cpu, 0, 5);
    ASSERT_FLAG_SET(cpu, FLAG_ZF);
}

static void test_cmp_greater_value_clears_zero_flag(void) {
    CPU cpu = {0};
    cpu.registers[0] = 10;
    execute_cmp(&cpu, 0, 3);
    ASSERT_FLAG_CLEAR(cpu, FLAG_ZF);
}

static void test_jmp_sets_pc_unconditionally(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    execute_jmp(&cpu, 0x42);
    ASSERT_EQ(0x42, cpu.pc);
}

static void test_jz_jumps_when_zero_flag_set(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = FLAG_ZF;
    execute_jz(&cpu, 0x80);
    ASSERT_EQ(0x80, cpu.pc);
}

static void test_jz_does_not_jump_when_zero_flag_clear(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    execute_jz(&cpu, 0x80);
    ASSERT_EQ(0x10, cpu.pc);
}

static void test_jnz_jumps_when_zero_flag_clear(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    execute_jnz(&cpu, 0x80);
    ASSERT_EQ(0x80, cpu.pc);
}

static void test_jnz_does_not_jump_when_zero_flag_set(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = FLAG_ZF;
    execute_jnz(&cpu, 0x80);
    ASSERT_EQ(0x10, cpu.pc);
}

static void test_jg_jumps_when_signed_greater(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = 0;
    execute_jg(&cpu, 0x80);
    ASSERT_EQ(0x80, cpu.pc);
}

static void test_jg_does_not_jump_on_equality(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = FLAG_ZF;
    execute_jg(&cpu, 0x80);
    ASSERT_EQ(0x10, cpu.pc);
}

static void test_jl_jumps_when_sign_differs_from_overflow(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = FLAG_SF;
    execute_jl(&cpu, 0x80);
    ASSERT_EQ(0x80, cpu.pc);
}

static void test_jl_does_not_jump_when_sign_matches_overflow(void) {
    CPU cpu = {0};
    cpu.pc = 0x10;
    cpu.flags = 0;
    execute_jl(&cpu, 0x80);
    ASSERT_EQ(0x10, cpu.pc);
}

static void test_call_pushes_return_address_and_jumps(void) {
    CPU cpu = {0};
    cpu.pc = 0x20;
    execute_call(&cpu, 0x90);
    ASSERT_EQ(0x90, cpu.pc);
    ASSERT_EQ(0xFF, cpu.sp);
    ASSERT_EQ(0x20, cpu.memory[cpu.sp]);
}

static void test_ret_restores_pc_from_stack(void) {
    CPU cpu = {0};
    cpu.pc = 0x20;
    execute_call(&cpu, 0x90);
    execute_ret(&cpu);
    ASSERT_EQ(0x20, cpu.pc);
    ASSERT_EQ(0, cpu.sp);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_halt_clears_running_flag);
    RUN_TEST(test_cmp_equal_values_set_zero_flag);
    RUN_TEST(test_cmp_greater_value_clears_zero_flag);
    RUN_TEST(test_jmp_sets_pc_unconditionally);
    RUN_TEST(test_jz_jumps_when_zero_flag_set);
    RUN_TEST(test_jz_does_not_jump_when_zero_flag_clear);
    RUN_TEST(test_jnz_jumps_when_zero_flag_clear);
    RUN_TEST(test_jnz_does_not_jump_when_zero_flag_set);
    RUN_TEST(test_jg_jumps_when_signed_greater);
    RUN_TEST(test_jg_does_not_jump_on_equality);
    RUN_TEST(test_jl_jumps_when_sign_differs_from_overflow);
    RUN_TEST(test_jl_does_not_jump_when_sign_matches_overflow);
    RUN_TEST(test_call_pushes_return_address_and_jumps);
    RUN_TEST(test_ret_restores_pc_from_stack);

    TEST_SUMMARY();
}
