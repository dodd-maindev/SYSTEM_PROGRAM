/**
 * @file test_demo.c
 * @brief Verbose demo — prints detailed input/output for each test case.
 *
 * Usage: ./matbench_demo
 * Shows matrices, expected vs actual results for presentation.
 */
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/* Shared counters from test_demo_main.c */
extern int g_pass;
extern int g_total;

/** Print PASS/FAIL with formatted output. */
static void check(int ok) {
    g_total++;
    if (ok) { g_pass++; }
    printf("  Result: %s\n\n", ok ? "[PASS]" : "[FAIL]");
}

/** Demo 1: Identity — A * I = A. */
void demo_identity(void) {
    printf("--- Test 1: Identity Matrix (A * I = A) ---\n");
    Matrix *a = matrix_create(3, 3);
    Matrix *id = matrix_create(3, 3);
    Matrix *c = matrix_create(3, 3);

    matrix_fill_random(a, 42);
    matrix_fill_zero(id);
    for (int i = 0; i < 3; i++) { matrix_set(id, i, i, 1.0); }

    matrix_print(a, "A");
    matrix_print(id, "I");
    multiply_naive(a, id, c);
    matrix_print(c, "C = A * I");
    check(verify_matrices(a, c, 1e-12));

    matrix_free(a); matrix_free(id); matrix_free(c);
}

/** Demo 2: Zero — A * 0 = 0. */
void demo_zero(void) {
    printf("--- Test 2: Zero Matrix (A * 0 = 0) ---\n");
    Matrix *a = matrix_create(3, 3);
    Matrix *z = matrix_create(3, 3);
    Matrix *c = matrix_create(3, 3);
    Matrix *expected = matrix_create(3, 3);

    matrix_fill_random(a, 7);
    matrix_fill_zero(z);
    matrix_fill_zero(expected);

    matrix_print(a, "A");
    matrix_print(z, "Zero");
    multiply_naive(a, z, c);
    matrix_print(c, "C = A * 0");
    check(verify_matrices(expected, c, 1e-12));

    matrix_free(a); matrix_free(z);
    matrix_free(c); matrix_free(expected);
}

/** Demo 3: Known 2x2. */
void demo_known_2x2(void) {
    printf("--- Test 3: Known 2x2 Result ---\n");
    Matrix *a = matrix_create(2, 2);
    Matrix *b = matrix_create(2, 2);
    Matrix *c = matrix_create(2, 2);

    double av[] = {1, 2, 3, 4};
    double bv[] = {5, 6, 7, 8};
    for (int i = 0; i < 4; i++) {
        a->data[i] = av[i]; b->data[i] = bv[i];
    }

    matrix_print(a, "A");
    matrix_print(b, "B");
    multiply_naive(a, b, c);
    matrix_print(c, "C = A * B");

    printf("  Expected: [[19, 22], [43, 50]]\n");
    int ok = (c->data[0] == 19.0 && c->data[1] == 22.0 &&
              c->data[2] == 43.0 && c->data[3] == 50.0);
    check(ok);

    matrix_free(a); matrix_free(b); matrix_free(c);
}
