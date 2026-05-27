/**
 * @file test_basic.c
 * @brief Basic matrix multiplication tests: identity, zero, known result.
 */
#include "matrix.h"
#include "multiply.h"
#include "verify.h"

#include <stdio.h>

/* Shared test infrastructure from test_main.c */
extern void assert_test(const char *name, int condition);

/** Test 1: Identity multiplication — A * I = A. */
static void test_identity(void) {
    int n = 4;
    Matrix *a = matrix_create(n, n);
    Matrix *id = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);

    matrix_fill_random(a, 99);
    matrix_fill_zero(id);
    for (int i = 0; i < n; i++) {
        matrix_set(id, i, i, 1.0);
    }

    multiply_naive(a, id, c);
    assert_test("Identity: A * I = A", verify_matrices(a, c, 1e-12));

    matrix_free(a);
    matrix_free(id);
    matrix_free(c);
}

/** Test 2: Zero multiplication — A * 0 = 0. */
static void test_zero(void) {
    int n = 4;
    Matrix *a = matrix_create(n, n);
    Matrix *zero = matrix_create(n, n);
    Matrix *c = matrix_create(n, n);
    Matrix *expected = matrix_create(n, n);

    matrix_fill_random(a, 7);
    matrix_fill_zero(zero);
    matrix_fill_zero(expected);

    multiply_naive(a, zero, c);
    assert_test("Zero: A * 0 = 0", verify_matrices(expected, c, 1e-12));

    matrix_free(a);
    matrix_free(zero);
    matrix_free(c);
    matrix_free(expected);
}

/** Test 3: Known 2x2 — [[1,2],[3,4]] * [[5,6],[7,8]] = [[19,22],[43,50]]. */
static void test_known_2x2(void) {
    Matrix *a = matrix_create(2, 2);
    Matrix *b = matrix_create(2, 2);
    Matrix *c = matrix_create(2, 2);

    double av[] = {1, 2, 3, 4};
    double bv[] = {5, 6, 7, 8};
    for (int i = 0; i < 4; i++) {
        a->data[i] = av[i];
        b->data[i] = bv[i];
    }

    multiply_naive(a, b, c);
    int ok = (c->data[0] == 19.0 && c->data[1] == 22.0 &&
              c->data[2] == 43.0 && c->data[3] == 50.0);
    assert_test("Known 2x2 result", ok);

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);
}

/** Test 4: Edge case — 1x1 matrix: 3 * 7 = 21. */
static void test_1x1(void) {
    Matrix *a = matrix_create(1, 1);
    Matrix *b = matrix_create(1, 1);
    Matrix *c = matrix_create(1, 1);

    a->data[0] = 3.0;
    b->data[0] = 7.0;

    multiply_naive(a, b, c);
    assert_test("1x1 edge case: 3*7=21", c->data[0] == 21.0);

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);
}

void test_basic_operations(void) {
    printf("[Basic Operations]\n");
    test_identity();
    test_zero();
    test_known_2x2();
    test_1x1();
}
