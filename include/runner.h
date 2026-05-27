/**
 * @file runner.h
 * @brief Benchmark runner — orchestrates all multiplication methods.
 */
#ifndef RUNNER_H
#define RUNNER_H

#include "csv_writer.h"

/** Configuration for a complete benchmark session. */
typedef struct {
    int *sizes;            /**< Array of matrix sizes to benchmark. */
    int num_sizes;         /**< Length of sizes array. */
    int *thread_counts;    /**< Array of thread counts to test. */
    int num_thread_counts; /**< Length of thread_counts array. */
    int block_size;        /**< Block size for tiled methods. */
    int run_verify;        /**< If non-zero, verify each result. */
    CsvWriter *csv;        /**< Optional CSV writer (NULL to skip). */
} RunnerConfig;

/** Execute all benchmarks according to the given configuration. */
void runner_execute(const RunnerConfig *config);

#endif /* RUNNER_H */
