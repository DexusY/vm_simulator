#include "test_framework.h"
#include "../vm_config.h"
#include "../instructions_stack.h"

static void test_psh_decrements_sp_and_stores_value(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 42);
    ASSERT_EQ(0xFF, cpu.sp);
    ASSERT_EQ(42, cpu.memory[cpu.sp]);
}

static void test_psh_multiple_values_grows_stack_downward(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 1);
    execute_psh(&cpu, 2);
    execute_psh(&cpu, 3);
    ASSERT_EQ(3, cpu.memory[cpu.sp]);
    ASSERT_EQ(2, cpu.memory[cpu.sp + 1]);
    ASSERT_EQ(1, cpu.memory[cpu.sp + 2]);
}

static void test_pop_advances_sp(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 7);
    execute_psh(&cpu, 8);
    execute_pop(&cpu);
    ASSERT_EQ(0xFF, cpu.sp);
}

static void test_pop_on_empty_stack_is_a_no_op(void) {
    CPU cpu = {0};
    execute_pop(&cpu);
    ASSERT_EQ(0, cpu.sp);
}

static void test_dup_duplicates_top_of_stack(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 9);
    execute_dup(&cpu);
    ASSERT_EQ(9, cpu.memory[cpu.sp]);
    ASSERT_EQ(9, cpu.memory[cpu.sp + 1]);
    ASSERT_EQ(0xFE, cpu.sp);
}

static void test_dup_on_empty_stack_is_a_no_op(void) {
    CPU cpu = {0};
    execute_dup(&cpu);
    ASSERT_EQ(0, cpu.sp);
}

static void test_swap_exchanges_top_two_elements(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 1);
    execute_psh(&cpu, 2);
    execute_swap(&cpu);
    ASSERT_EQ(1, cpu.memory[cpu.sp]);
    ASSERT_EQ(2, cpu.memory[cpu.sp + 1]);
}

static void test_swap_with_less_than_two_elements_is_a_no_op(void) {
    CPU cpu = {0};
    execute_psh(&cpu, 5);
    uint8_t before = cpu.memory[cpu.sp];
    execute_swap(&cpu);
    ASSERT_EQ(before, cpu.memory[cpu.sp]);
}

static void test_nop_does_not_change_state(void) {
    CPU cpu = {0};
    cpu.pc = 10;
    cpu.sp = 20;
    cpu.flags = 0xFF;
    execute_nop(&cpu);
    ASSERT_EQ(10, cpu.pc);
    ASSERT_EQ(20, cpu.sp);
    ASSERT_EQ(0xFF, cpu.flags);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_psh_decrements_sp_and_stores_value);
    RUN_TEST(test_psh_multiple_values_grows_stack_downward);
    RUN_TEST(test_pop_advances_sp);
    RUN_TEST(test_pop_on_empty_stack_is_a_no_op);
    RUN_TEST(test_dup_duplicates_top_of_stack);
    RUN_TEST(test_dup_on_empty_stack_is_a_no_op);
    RUN_TEST(test_swap_exchanges_top_two_elements);
    RUN_TEST(test_swap_with_less_than_two_elements_is_a_no_op);
    RUN_TEST(test_nop_does_not_change_state);

    TEST_SUMMARY();
}
