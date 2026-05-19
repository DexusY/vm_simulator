#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

static int tests_run    = 0;
static int tests_failed = 0;
static int current_test_failed = 0;

#define ASSERT_TRUE(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            fprintf(stderr, "    FAIL %s:%d: expected true: %s\n",             \
                    __FILE__, __LINE__, #cond);                                \
            current_test_failed = 1;                                           \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_EQ(expected, actual)                                            \
    do {                                                                       \
        long long _e = (long long)(expected);                                  \
        long long _a = (long long)(actual);                                    \
        if (_e != _a) {                                                        \
            fprintf(stderr,                                                    \
                    "    FAIL %s:%d: expected %lld, got %lld (%s)\n",          \
                    __FILE__, __LINE__, _e, _a, #actual);                      \
            current_test_failed = 1;                                           \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_FLAG_SET(cpu, flag)   ASSERT_TRUE(((cpu).flags & (flag)) != 0)
#define ASSERT_FLAG_CLEAR(cpu, flag) ASSERT_TRUE(((cpu).flags & (flag)) == 0)

#define RUN_TEST(test_fn)                                                      \
    do {                                                                       \
        current_test_failed = 0;                                               \
        tests_run++;                                                           \
        test_fn();                                                             \
        if (current_test_failed) {                                             \
            tests_failed++;                                                    \
            fprintf(stderr, "  [FAIL] %s\n", #test_fn);                        \
        } else {                                                               \
            fprintf(stderr, "  [PASS] %s\n", #test_fn);                        \
        }                                                                      \
    } while (0)

#define TEST_SUMMARY()                                                         \
    do {                                                                       \
        fprintf(stderr, "\n%d run, %d failed\n", tests_run, tests_failed);     \
        return tests_failed == 0 ? 0 : 1;                                      \
    } while (0)

// Redirects the simulator's verbose printfs away from the test report.
// Call once at the top of main() in each test binary.
static inline void silence_stdout(void) {
    if (!freopen("/dev/null", "w", stdout)) { /* ignore */ }
}

#endif
