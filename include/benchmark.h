/**
 * @file benchmark.h
 * @brief High-resolution timing utilities for performance measurement.
 */
#ifndef BENCHMARK_H
#define BENCHMARK_H

/** Get current wall-clock time in seconds (CLOCK_MONOTONIC). */
double benchmark_get_time(void);

/** Calculate GFLOPS for NxN matrix multiplication given elapsed time. */
double benchmark_calc_gflops(int n, double elapsed_seconds);

/** Print a formatted benchmark result line to stdout. */
void benchmark_print_result(const char *method, int n, int threads,
                            double elapsed, double gflops);

/** Print the benchmark table header. */
void benchmark_print_header(void);

/** Print a separator line for the benchmark table. */
void benchmark_print_separator(void);

#endif /* BENCHMARK_H */
