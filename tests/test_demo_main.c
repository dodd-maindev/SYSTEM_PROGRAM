/**
 * @file test_demo_main.c
 * @brief Demo entry point — verbose test runner for presentation.
 *
 * Usage: ./matbench_demo
 * Prints detailed input/output/timing for every test case.
 */
#include <stdio.h>

/* Shared counters */
int g_pass = 0;
int g_total = 0;

/* Demo functions from other files */
void demo_identity(void);
void demo_zero(void);
void demo_known_2x2(void);
void demo_method_correctness(void);
void demo_1x1(void);

int main(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  MatBench — Verbose Test Demo                ║\n");
    printf("║  Parallel Matrix Multiplication Benchmark    ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    demo_identity();
    demo_zero();
    demo_known_2x2();
    demo_1x1();
    demo_method_correctness();

    printf("══════════════════════════════════════════════\n");
    printf("  Final: %d / %d tests passed\n", g_pass, g_total);
    printf("══════════════════════════════════════════════\n\n");

    return (g_pass == g_total) ? 0 : 1;
}
