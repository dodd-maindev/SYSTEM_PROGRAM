# MatBench — Parallel Matrix Multiplication Benchmark

## Project Title
**C1: Parallel Matrix Operations** — System Programming Final Project (VNU-IS)

## Group Members

| Name            | Student ID | Email               |
|-----------------|-----------|---------------------|
| Vu Quang Hung   | [ID]      | [email]             |
| Dao Duc Do      | [ID]      | [email]             |

## Overview

A CLI benchmark suite that implements **6 variants** of dense NxN matrix
multiplication in C, progressively demonstrating optimization techniques:

1. **Naive (i,j,k)** — Textbook triple-loop, cache-unfriendly baseline
2. **Loop-swapped (i,k,j)** — Stride-1 access on B for better cache locality
3. **Cache-blocked (Tiled)** — Divide into sub-blocks fitting in L1/L2 cache
4. **Threaded (pthreads)** — Row-wise stripe decomposition across N threads
5. **Combined (Threaded + Tiled)** — Final optimized version
6. **BLAS dgemm** — Professional-grade comparison (optional)

---

## Prerequisites & Setup

### Required
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential    # GCC, make, libc
```

### Optional (for BLAS comparison)
```bash
sudo apt install libopenblas-dev    # or: sudo apt install libatlas-base-dev
```

### Optional (for generating plots)
```bash
pip3 install matplotlib
```

### System Requirements
- **OS**: Linux x86-64 (or WSL on Windows)
- **Compiler**: GCC with C11 support
- **Libraries**: pthreads (included in POSIX)

---

## Build Instructions

```bash
# Build the benchmark binary
make all

# Build with BLAS comparison support
make USE_BLAS=1

# Clean all build artifacts
make clean
```

The project compiles with `-Wall -Wextra -std=c11` and produces **zero warnings**.

---

## Usage Examples

### Quick Start
```bash
# Default benchmark (sizes: 128,256,512 | threads: 1,2,4,8 | block: 64)
./matbench
```

### Custom Configuration
```bash
# Specify matrix sizes and thread counts
./matbench --sizes 256,512,1024,2048 --threads 1,2,4,8

# Enable verification + export CSV
./matbench --sizes 128,512,1024 --verify --csv results/benchmark.csv

# Tune tile block size (try 16, 32, 64, 128)
./matbench --block 32 --sizes 512,1024

# Show help
./matbench --help
```

### CLI Options Reference

| Option            | Default       | Description                          |
|-------------------|---------------|--------------------------------------|
| `--sizes S1,S2`   | 128,256,512   | Comma-separated matrix dimensions    |
| `--threads T1,T2` | 1,2,4,8       | Thread counts for parallel methods   |
| `--block B`        | 64            | Tile block size for cache-blocking   |
| `--verify`         | OFF           | Verify all results vs naive baseline |
| `--csv <file>`     | (none)        | Export results to CSV file           |

---

## Testing Instructions

```bash
# Run the full test suite (8 test cases)
make test
```

### Test Cases

| #  | Test Name                       | What it Verifies                  |
|----|---------------------------------|-----------------------------------|
| 1  | Identity: A × I = A            | Multiplication with identity      |
| 2  | Zero: A × 0 = 0                | Multiplication with zero matrix   |
| 3  | Known 2×2 result               | Hand-calculated expected output   |
| 4  | 1×1 edge case                  | Minimal matrix dimension          |
| 5  | IKJ matches naive              | Loop-swapped correctness (N=64)   |
| 6  | Tiled matches naive             | Cache-blocked correctness (N=64)  |
| 7  | Parallel(2t) matches naive      | Threaded correctness (N=64)       |
| 8  | Combined(2t,B=16) matches naive | Final version correctness (N=64)  |

All tests compare against the naive baseline with **tolerance = 1e-9**.
Output clearly reports `[PASS]` or `[FAIL]` for each case.

---

## Interpreting Benchmark Results

### Console Output

The benchmark prints a table for each matrix size:

```
Method               |     N | Thr |     Time (s) |       GFLOPS
---------------------+-------+-----+--------------+-------------
naive_ijk            |   512 |   1 |     0.450846 |       0.5954
ikj_swapped          |   512 |   1 |     0.094956 |       2.8269
tiled                |   512 |   1 |     0.068582 |       3.9141
parallel_4t          |   512 |   4 |     0.026301 |      10.2064
combined_4t          |   512 |   4 |     0.026815 |      10.0106
```

- **Time (s)**: Wall-clock time using `CLOCK_MONOTONIC`
- **GFLOPS**: Billions of floating-point ops per second (`2*N³ / time / 1e9`)
- Higher GFLOPS = better performance

### Key Observations to Look For

1. **Naive → IKJ**: Large speedup from just reordering loops (cache locality)
2. **IKJ → Tiled**: Further gain from fitting blocks in L1/L2 cache
3. **1 thread → N threads**: Speedup curve (should approach linear, then flatten)
4. **Combined vs BLAS**: Gap shows impact of SIMD, prefetch, micro-kernels

### Generating Plots

```bash
# Run benchmark with CSV export
./matbench --sizes 128,256,512,1024 --threads 1,2,4,8 \
           --verify --csv results/benchmark.csv

# Generate speedup curves and GFLOPS comparison charts
python3 scripts/plot_results.py results/benchmark.csv
```

Output plots saved to `results/`:
- `speedup_threads.png` — Speedup vs thread count (with ideal linear reference)
- `gflops_comparison.png` — Bar chart comparing all methods at largest N

---

## Project Structure

```
matbench/
├── include/                    # Public header files
│   ├── matrix.h                # Matrix struct + inline accessors
│   ├── multiply.h              # All 6 method declarations
│   ├── benchmark.h             # Timing utilities
│   ├── verify.h                # Verification interface
│   ├── csv_writer.h            # CSV export interface
│   └── runner.h                # Benchmark orchestrator config
├── src/                        # Implementation (each file < 100 lines)
│   ├── main.c                  # CLI entry point + argument parsing
│   ├── runner.c                # Benchmark execution loop
│   ├── matrix.c                # Matrix create/free/fill
│   ├── multiply_naive.c        # Method 1: Naive (i,j,k)
│   ├── multiply_ikj.c          # Method 2: Loop-swapped (i,k,j)
│   ├── multiply_tiled.c        # Method 3: Cache-blocked
│   ├── multiply_parallel.c     # Method 4: pthreads row-wise
│   ├── multiply_combined.c     # Method 5: Threaded + Tiled
│   ├── multiply_blas.c         # Method 6: BLAS wrapper
│   ├── benchmark.c             # CLOCK_MONOTONIC timer
│   ├── verify.c                # Element-wise comparison
│   └── csv_writer.c            # CSV file output
├── tests/                      # Test suite (8 cases)
│   ├── test_main.c             # Test runner entry point
│   ├── test_basic.c            # Tests 1-4: basic operations
│   └── test_methods.c          # Tests 5-8: cross-method verification
├── scripts/
│   └── plot_results.py         # matplotlib speedup/GFLOPS plots
├── results/                    # Generated CSVs and PNG plots
├── Makefile                    # Targets: all, clean, test, benchmark
├── .gitignore
└── README.md
```

---

## Individual Contributions

- **Vu Quang Hung**: [Description of contributions]
- **Dao Duc Do**: [Description of contributions]
