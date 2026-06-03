/**
 * @file test_demo_methods.c
 * @brief Verbose demo — cross-method verification with timing.
 *
 * Shows that all optimized methods produce the same result as naive,
 * with elapsed time for each method.
 */
#define _POSIX_C_SOURCE 199309L

#include "benchmark.h"
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/* Shared counters from test_demo.c */
extern int g_pass;
extern int g_total;

/** Run one method, compare vs reference, print time and verdict. */
static void verify_method(const char *name, const Matrix *ref,
                          const Matrix *c, double elapsed) {
    int ok = verify_matrices(ref, c, VERIFY_TOLERANCE);
    g_total++;
    if (ok) { g_pass++; }
    printf("  %-22s | %10.6f s | %s\n",
           name, elapsed, ok ? "MATCH ✓" : "MISMATCH ✗");
}

/** Demo 4-8: All methods match naive baseline (N=64). */
void demo_method_correctness(void) {
    int n = 64;
    printf("━━━ Tests 4-8: Cross-Method Verification (N=%d) ━━━\n", n);
    printf("  All methods compared against naive (i,j,k) baseline.\n");
    printf("  Tolerance: %.0e\n\n", VERIFY_TOLERANCE);

    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 84);

    printf("  %-22s | %10s | %s\n", "Method", "Time", "Status");
    printf("  %-22s-+-%10s-+-%s\n", "----------------------",
           "----------", "----------");

    /* Naive baseline */
    double t0 = benchmark_get_time();
    multiply_naive(a, b, ref);
    double t1 = benchmark_get_time();
    g_total++; g_pass++;
    printf("  %-22s | %10.6f s | BASELINE\n", "naive (i,j,k)", t1 - t0);

    /* IKJ */
    t0 = benchmark_get_time();
    multiply_ikj(a, b, c);
    t1 = benchmark_get_time();
    verify_method("ikj (loop-swapped)", ref, c, t1 - t0);

    /* Tiled */
    t0 = benchmark_get_time();
    multiply_tiled(a, b, c, 16);
    t1 = benchmark_get_time();
    verify_method("tiled (block=16)", ref, c, t1 - t0);

    /* Parallel 2 threads */
    t0 = benchmark_get_time();
    multiply_parallel(a, b, c, 2);
    t1 = benchmark_get_time();
    verify_method("parallel (2 threads)", ref, c, t1 - t0);

    /* Combined 2 threads */
    t0 = benchmark_get_time();
    multiply_combined(a, b, c, 2, 16);
    t1 = benchmark_get_time();
    verify_method("combined (2t, B=16)", ref, c, t1 - t0);

    printf("\n");

    matrix_free(a); matrix_free(b);
    matrix_free(ref); matrix_free(c);
}

/** Demo edge: 1x1 matrix. */
void demo_1x1(void) {
    printf("━━━ Test 9: Edge Case — 1×1 Matrix ━━━\n");
    Matrix *a = matrix_create(1, 1);
    Matrix *b = matrix_create(1, 1);
    Matrix *c = matrix_create(1, 1);

    a->data[0] = 3.0;
    b->data[0] = 7.0;

    multiply_naive(a, b, c);
    printf("  A = [3.0],  B = [7.0]\n");
    printf("  C = A × B = [%.1f]\n", c->data[0]);
    printf("  Expected:   [21.0]\n");

    g_total++;
    int ok = (c->data[0] == 21.0);
    if (ok) { g_pass++; }
    printf("  Result: %s\n\n", ok ? "[PASS] ✓" : "[FAIL] ✗");

    matrix_free(a); matrix_free(b); matrix_free(c);
}
