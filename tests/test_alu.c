#include "test_framework.h"
#include "../vm_config.h"
#include "../instructions_alu.h"
#include "../instructions_stack.h"

static CPU make_cpu_with_stack(const uint8_t *values, int count) {
    CPU cpu = {0};
    for (int i = 0; i < count; i++) {
        execute_psh(&cpu, values[i]);
    }
    return cpu;
}

static void test_add_basic(void) {
    uint8_t stack[] = {5, 6};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_add(&cpu);
    ASSERT_EQ(11, cpu.memory[cpu.sp]);
}

static void test_add_sets_zero_flag_on_zero_result(void) {
    uint8_t stack[] = {0, 0};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_add(&cpu);
    ASSERT_FLAG_SET(cpu, FLAG_ZF);
}

static void test_add_sets_carry_flag_on_unsigned_overflow(void) {
    uint8_t stack[] = {0xFF, 0x01};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_add(&cpu);
    ASSERT_FLAG_SET(cpu, FLAG_CF);
    ASSERT_EQ(0, cpu.memory[cpu.sp]);
}

static void test_add_sets_overflow_flag_on_signed_overflow(void) {
    uint8_t stack[] = {1, 127};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_add(&cpu);
    ASSERT_FLAG_SET(cpu, FLAG_OF);
    ASSERT_FLAG_SET(cpu, FLAG_SF);
}

static void test_sub_basic(void) {
    uint8_t stack[] = {3, 10};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_sub(&cpu);
    ASSERT_EQ(7, cpu.memory[cpu.sp]);
}

static void test_sub_equal_operands_sets_zero_flag(void) {
    uint8_t stack[] = {42, 42};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_sub(&cpu);
    ASSERT_FLAG_SET(cpu, FLAG_ZF);
    ASSERT_EQ(0, cpu.memory[cpu.sp]);
}

static void test_mul_basic(void) {
    uint8_t stack[] = {4, 5};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_mul(&cpu);
    ASSERT_EQ(20, cpu.memory[cpu.sp]);
}

static void test_mul_carry_flag_on_wide_result(void) {
    uint8_t stack[] = {16, 16};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_mul(&cpu);
    ASSERT_FLAG_SET(cpu, FLAG_CF);
}

static void test_div_basic(void) {
    uint8_t stack[] = {3, 12};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_div(&cpu);
    ASSERT_EQ(4, cpu.memory[cpu.sp]);
}

static void test_div_by_zero_does_not_push_result(void) {
    uint8_t stack[] = {0, 7};
    CPU cpu = make_cpu_with_stack(stack, 2);
    uint8_t sp_before = cpu.sp;
    execute_div(&cpu);
    ASSERT_EQ((uint8_t)(sp_before + 2), cpu.sp);
}

static void test_mod_basic(void) {
    uint8_t stack[] = {3, 10};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_mod(&cpu);
    ASSERT_EQ(1, cpu.memory[cpu.sp]);
}

static void test_mod_by_zero_does_not_push_result(void) {
    uint8_t stack[] = {0, 5};
    CPU cpu = make_cpu_with_stack(stack, 2);
    uint8_t sp_before = cpu.sp;
    execute_mod(&cpu);
    ASSERT_EQ((uint8_t)(sp_before + 2), cpu.sp);
}

static void test_and_basic(void) {
    uint8_t stack[] = {0x0F, 0xF0};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_and(&cpu);
    ASSERT_EQ(0x00, cpu.memory[cpu.sp]);
    ASSERT_FLAG_SET(cpu, FLAG_ZF);
}

static void test_or_basic(void) {
    uint8_t stack[] = {0x0F, 0xF0};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_or(&cpu);
    ASSERT_EQ(0xFF, cpu.memory[cpu.sp]);
}

static void test_xor_basic(void) {
    uint8_t stack[] = {0xAA, 0xFF};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_xor(&cpu);
    ASSERT_EQ(0x55, cpu.memory[cpu.sp]);
}

static void test_not_basic(void) {
    uint8_t stack[] = {0x0F};
    CPU cpu = make_cpu_with_stack(stack, 1);
    execute_not(&cpu);
    ASSERT_EQ(0xF0, cpu.memory[cpu.sp]);
    ASSERT_FLAG_SET(cpu, FLAG_SF);
}

static void test_shl_basic(void) {
    uint8_t stack[] = {2, 0x01};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_shl(&cpu);
    ASSERT_EQ(0x04, cpu.memory[cpu.sp]);
}

static void test_shl_out_of_range_does_not_push_result(void) {
    uint8_t stack[] = {8, 0x01};
    CPU cpu = make_cpu_with_stack(stack, 2);
    uint8_t sp_before = cpu.sp;
    execute_shl(&cpu);
    ASSERT_EQ((uint8_t)(sp_before + 2), cpu.sp);
}

static void test_shr_basic(void) {
    uint8_t stack[] = {1, 0x08};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_shr(&cpu);
    ASSERT_EQ(0x04, cpu.memory[cpu.sp]);
}

static void test_shr_sets_carry_when_bit_shifted_out(void) {
    uint8_t stack[] = {1, 0x03};
    CPU cpu = make_cpu_with_stack(stack, 2);
    execute_shr(&cpu);
    ASSERT_EQ(0x01, cpu.memory[cpu.sp]);
    ASSERT_FLAG_SET(cpu, FLAG_CF);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_add_basic);
    RUN_TEST(test_add_sets_zero_flag_on_zero_result);
    RUN_TEST(test_add_sets_carry_flag_on_unsigned_overflow);
    RUN_TEST(test_add_sets_overflow_flag_on_signed_overflow);
    RUN_TEST(test_sub_basic);
    RUN_TEST(test_sub_equal_operands_sets_zero_flag);
    RUN_TEST(test_mul_basic);
    RUN_TEST(test_mul_carry_flag_on_wide_result);
    RUN_TEST(test_div_basic);
    RUN_TEST(test_div_by_zero_does_not_push_result);
    RUN_TEST(test_mod_basic);
    RUN_TEST(test_mod_by_zero_does_not_push_result);
    RUN_TEST(test_and_basic);
    RUN_TEST(test_or_basic);
    RUN_TEST(test_xor_basic);
    RUN_TEST(test_not_basic);
    RUN_TEST(test_shl_basic);
    RUN_TEST(test_shl_out_of_range_does_not_push_result);
    RUN_TEST(test_shr_basic);
    RUN_TEST(test_shr_sets_carry_when_bit_shifted_out);

    TEST_SUMMARY();
}
