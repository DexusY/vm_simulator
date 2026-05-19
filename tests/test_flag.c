#include "test_framework.h"
#include "../vm_config.h"
#include "../flag.h"

static void test_zero_result_sets_zero_flag_only(void) {
    CPU cpu = {0};
    update_flags(&cpu, 0, 0, 0);
    ASSERT_FLAG_SET(cpu, FLAG_ZF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_CF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_SF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_OF);
}

static void test_carry_flag_set_when_result_exceeds_byte(void) {
    CPU cpu = {0};
    update_flags(&cpu, 0x100, 0xFF, 0x01);
    ASSERT_FLAG_SET(cpu, FLAG_CF);
}

static void test_sign_flag_set_when_msb_set(void) {
    CPU cpu = {0};
    update_flags(&cpu, 0x80, 0x40, 0x40);
    ASSERT_FLAG_SET(cpu, FLAG_SF);
}

static void test_signed_overflow_when_two_positives_yield_negative(void) {
    CPU cpu = {0};
    update_flags(&cpu, 0x80, 0x40, 0x40);
    ASSERT_FLAG_SET(cpu, FLAG_OF);
}

static void test_no_signed_overflow_for_mixed_sign_operands(void) {
    CPU cpu = {0};
    update_flags(&cpu, 0x00, 0xFF, 0x01);
    ASSERT_FLAG_CLEAR(cpu, FLAG_OF);
}

static void test_update_flags_resets_previous_flags(void) {
    CPU cpu = {0};
    cpu.flags = FLAG_ZF | FLAG_CF | FLAG_SF | FLAG_OF;
    update_flags(&cpu, 1, 1, 0);
    ASSERT_FLAG_CLEAR(cpu, FLAG_ZF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_CF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_SF);
    ASSERT_FLAG_CLEAR(cpu, FLAG_OF);
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_zero_result_sets_zero_flag_only);
    RUN_TEST(test_carry_flag_set_when_result_exceeds_byte);
    RUN_TEST(test_sign_flag_set_when_msb_set);
    RUN_TEST(test_signed_overflow_when_two_positives_yield_negative);
    RUN_TEST(test_no_signed_overflow_for_mixed_sign_operands);
    RUN_TEST(test_update_flags_resets_previous_flags);

    TEST_SUMMARY();
}
