/**
 * @file test_main.c
 * @brief Test suite entry point — runs all test modules.
 *
 * Run via: make test
 * Reports PASS/FAIL for each test case with summary.
 */
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/** Global test counters (shared across test modules). */
int g_tests_passed = 0;
int g_tests_total = 0;

/** Assert helper: check condition and print result. */
void assert_test(const char *name, int condition) {
    g_tests_total++;
    if (condition) {
        g_tests_passed++;
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
    }
}

/* Test functions declared in separate files */
void test_basic_operations(void);
void test_method_correctness(void);

int main(void) {
    printf("=== MatBench Test Suite ===\n\n");

    test_basic_operations();
    test_method_correctness();

    printf("\n=== Results: %d / %d passed ===\n",
           g_tests_passed, g_tests_total);

    return (g_tests_passed == g_tests_total) ? 0 : 1;
}
