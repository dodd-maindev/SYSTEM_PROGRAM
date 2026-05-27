/**
 * @file verify.c
 * @brief Element-wise matrix comparison for correctness verification.
 */
#include "verify.h"

#include <math.h>
#include <stdio.h>

int verify_matrices(const Matrix *expected, const Matrix *actual,
                    double tolerance) {
    if (expected->rows != actual->rows ||
        expected->cols != actual->cols) {
        fprintf(stderr, "[FAIL] Dimension mismatch: (%d x %d) vs (%d x %d)\n",
                expected->rows, expected->cols,
                actual->rows, actual->cols);
        return 0;
    }

    int total = expected->rows * expected->cols;

    for (int i = 0; i < total; i++) {
        double diff = fabs(expected->data[i] - actual->data[i]);

        if (diff > tolerance) {
            int row = i / expected->cols;
            int col = i % expected->cols;
            fprintf(stderr, "[FAIL] at (%d,%d): expected=%.12f, "
                    "actual=%.12f, diff=%.2e\n",
                    row, col, expected->data[i],
                    actual->data[i], diff);
            return 0;
        }
    }
    return 1;
}
