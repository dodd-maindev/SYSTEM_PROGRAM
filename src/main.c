/**
 * @file main.c
 * @brief Entry point — CLI argument parsing and benchmark dispatch.
 *
 * Usage:
 *   ./matbench [options]
 *   --sizes 128,256,512   Matrix sizes to benchmark (default: 128,256,512)
 *   --threads 1,2,4,8     Thread counts to test (default: 1,2,4,8)
 *   --block 64            Block size for tiling (default: 64)
 *   --verify              Enable correctness verification
 *   --csv <file>          Output results to CSV file
 */
#include "csv_writer.h"
#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Default benchmark configuration values. */
#define DEFAULT_BLOCK_SIZE 64
#define MAX_PARAMS 16

/** Parse comma-separated integers into an array. Return count. */
static int parse_int_list(const char *str, int *out, int max) {
    int count = 0;
    char buf[256];
    strncpy(buf, str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *token = strtok(buf, ",");
    while (token && count < max) {
        out[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    return count;
}

/** Print usage help message. */
static void print_usage(const char *prog) {
    printf("Usage: %s [options]\n"
           "  --sizes S1,S2,...   Matrix sizes (default: 128,256,512)\n"
           "  --threads T1,...    Thread counts (default: 1,2,4,8)\n"
           "  --block B          Tile block size (default: 64)\n"
           "  --verify           Verify results against naive baseline\n"
           "  --csv <file>       Write results to CSV\n"
           "  --help             Show this message\n", prog);
}

int main(int argc, char *argv[]) {
    int sizes[MAX_PARAMS] = {128, 256, 512};
    int num_sizes = 3;
    int threads[MAX_PARAMS] = {1, 2, 4, 8};
    int num_threads = 4;
    int block_size = DEFAULT_BLOCK_SIZE;
    int verify = 0;
    const char *csv_path = NULL;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--sizes") == 0 && i + 1 < argc) {
            num_sizes = parse_int_list(argv[++i], sizes, MAX_PARAMS);
        } else if (strcmp(argv[i], "--threads") == 0 && i + 1 < argc) {
            num_threads = parse_int_list(argv[++i], threads, MAX_PARAMS);
        } else if (strcmp(argv[i], "--block") == 0 && i + 1 < argc) {
            block_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--verify") == 0) {
            verify = 1;
        } else if (strcmp(argv[i], "--csv") == 0 && i + 1 < argc) {
            csv_path = argv[++i];
        } else {
            print_usage(argv[0]);
            return (strcmp(argv[i], "--help") == 0) ? 0 : 1;
        }
    }

    printf("=== MatBench: Parallel Matrix Multiplication Benchmark ===\n");
    printf("Block size: %d | Verify: %s\n",
           block_size, verify ? "ON" : "OFF");

    CsvWriter *csv = csv_path ? csv_writer_open(csv_path) : NULL;

    RunnerConfig config = {
        .sizes = sizes,
        .num_sizes = num_sizes,
        .thread_counts = threads,
        .num_thread_counts = num_threads,
        .block_size = block_size,
        .run_verify = verify,
        .csv = csv,
    };

    runner_execute(&config);

    if (csv) {
        csv_writer_close(csv);
        printf("\nResults saved to: %s\n", csv_path);
    }

    printf("\nDone.\n");
    return 0;
}
