#include "test_framework.h"
#include "../program_loader.h"

static void test_loader_copies_bytes_into_memory(void) {
    const uint8_t program[] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t memory[256] = {0};

    loader(program, (int)sizeof(program), memory);

    ASSERT_EQ(0xAA, memory[0]);
    ASSERT_EQ(0xBB, memory[1]);
    ASSERT_EQ(0xCC, memory[2]);
    ASSERT_EQ(0xDD, memory[3]);
}

static void test_loader_leaves_remaining_memory_untouched(void) {
    const uint8_t program[] = {0x01, 0x02};
    uint8_t memory[256];
    for (int i = 0; i < 256; i++) memory[i] = 0xEE;

    loader(program, (int)sizeof(program), memory);

    for (int i = (int)sizeof(program); i < 256; i++) {
        ASSERT_EQ(0xEE, memory[i]);
    }
}

static void test_loader_handles_empty_program(void) {
    uint8_t memory[256];
    for (int i = 0; i < 256; i++) memory[i] = 0xEE;

    loader(NULL, 0, memory);

    for (int i = 0; i < 256; i++) {
        ASSERT_EQ(0xEE, memory[i]);
    }
}

int main(void) {
    silence_stdout();

    RUN_TEST(test_loader_copies_bytes_into_memory);
    RUN_TEST(test_loader_leaves_remaining_memory_untouched);
    RUN_TEST(test_loader_handles_empty_program);

    TEST_SUMMARY();
}
