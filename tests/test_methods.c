/**
 * @file test_methods.c
 * @brief Cross-method correctness tests — all methods must match naive.
 */
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/* Shared test infrastructure from test_main.c */
extern void assert_test(const char *name, int condition);

/** Test 5: IKJ matches naive reference (N=64). */
static void test_ikj_correctness(void) {
    int n = 64;
    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 84);
    multiply_naive(a, b, ref);

    multiply_ikj(a, b, c);
    assert_test("IKJ matches naive (N=64)",
                verify_matrices(ref, c, 1e-9));

    matrix_free(a);
    matrix_free(b);
    matrix_free(ref);
    matrix_free(c);
}

/** Test 6: Tiled matches naive reference (N=64, block=16). */
static void test_tiled_correctness(void) {
    int n = 64;
    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 84);
    multiply_naive(a, b, ref);

    multiply_tiled(a, b, c, 16);
    assert_test("Tiled matches naive (N=64, B=16)",
                verify_matrices(ref, c, 1e-9));

    matrix_free(a);
    matrix_free(b);
    matrix_free(ref);
    matrix_free(c);
}

/** Test 7: Parallel matches naive (N=64, 2 threads). */
static void test_parallel_correctness(void) {
    int n = 64;
    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 84);
    multiply_naive(a, b, ref);

    multiply_parallel(a, b, c, 2);
    assert_test("Parallel(2t) matches naive (N=64)",
                verify_matrices(ref, c, 1e-9));

    matrix_free(a);
    matrix_free(b);
    matrix_free(ref);
    matrix_free(c);
}

/** Test 8: Combined matches naive (N=64, 2 threads, block=16). */
static void test_combined_correctness(void) {
    int n = 64;
    Matrix *a = matrix_create(n, n);
    Matrix *b = matrix_create(n, n);
    Matrix *ref = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 42);
    matrix_fill_random(b, 84);
    multiply_naive(a, b, ref);

    multiply_combined(a, b, c, 2, 16);
    assert_test("Combined(2t,B=16) matches naive (N=64)",
                verify_matrices(ref, c, 1e-9));

    matrix_free(a);
    matrix_free(b);
    matrix_free(ref);
    matrix_free(c);
}

void test_method_correctness(void) {
    printf("\n[Method Correctness]\n");
    test_ikj_correctness();
    test_tiled_correctness();
    test_parallel_correctness();
    test_combined_correctness();
}
