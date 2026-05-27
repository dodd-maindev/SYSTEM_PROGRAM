/**
 * @file verify.h
 * @brief Matrix verification utilities for correctness testing.
 */
#ifndef VERIFY_H
#define VERIFY_H

#include "matrix.h"

/** Default tolerance for floating-point comparison. */
#define VERIFY_TOLERANCE 1e-9

/**
 * Compare two matrices element-wise within a given tolerance.
 * @return 1 if all elements match within tolerance, 0 otherwise.
 */
int verify_matrices(const Matrix *expected, const Matrix *actual,
                    double tolerance);

#endif /* VERIFY_H */
