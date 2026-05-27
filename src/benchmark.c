/**
 * @file benchmark.c
 * @brief High-resolution timing and formatted output for benchmarks.
 */
#define _POSIX_C_SOURCE 199309L

#include "benchmark.h"

#include <stdio.h>
#include <time.h>

double benchmark_get_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

double benchmark_calc_gflops(int n, double elapsed_seconds) {
    /* Matrix multiply: 2*N^3 floating-point operations */
    double flops = 2.0 * (double)n * (double)n * (double)n;

    if (elapsed_seconds <= 0.0) {
        return 0.0;
    }
    return flops / (elapsed_seconds * 1e9);
}

void benchmark_print_header(void) {
    printf("\n  %-20s | %5s | %3s | %12s | %12s\n",
           "Method", "N", "Thr", "Time (s)", "GFLOPS");
}

void benchmark_print_separator(void) {
    printf("  %-20s-+-%5s-+-%3s-+-%12s-+-%12s\n",
           "--------------------", "-----", "---",
           "------------", "------------");
}

void benchmark_print_result(const char *method, int n, int threads,
                            double elapsed, double gflops) {
    printf("  %-20s | %5d | %3d | %12.6f | %12.4f\n",
           method, n, threads, elapsed, gflops);
}
