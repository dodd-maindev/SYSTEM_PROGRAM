#!/usr/bin/env python3
"""
plot_results.py — Generate speedup and GFLOPS charts from benchmark CSV.

Usage:
    python3 scripts/plot_results.py results/benchmark.csv

Outputs PNG plots to the results/ directory.
"""
import sys
import csv
import os

try:
    import matplotlib.pyplot as plt
except ImportError:
    print("Error: matplotlib required. Install: pip install matplotlib")
    sys.exit(1)


def load_csv(filepath):
    """Load benchmark CSV into a list of dicts."""
    with open(filepath, "r") as f:
        return list(csv.DictReader(f))


def plot_speedup_by_threads(data, output_dir):
    """Plot speedup vs thread count for parallel/combined methods."""
    fig, ax = plt.subplots(figsize=(10, 6))
    sizes = sorted(set(int(r["matrix_size"]) for r in data))

    for size in sizes:
        rows = [r for r in data
                if r["method"].startswith("combined_")
                and int(r["matrix_size"]) == size]
        if not rows:
            continue
        threads = [int(r["threads"]) for r in rows]
        speedups = [float(r["speedup"]) for r in rows]
        ax.plot(threads, speedups, "o-", label=f"N={size}")

    ax.plot([1, 8], [1, 8], "k--", alpha=0.3, label="Ideal linear")
    ax.set_xlabel("Thread Count")
    ax.set_ylabel("Speedup (vs naive baseline)")
    ax.set_title("Speedup Curve — Combined (Threaded + Tiled)")
    ax.legend()
    ax.grid(True, alpha=0.3)
    fig.savefig(os.path.join(output_dir, "speedup_threads.png"), dpi=150)
    plt.close(fig)
    print(f"  Saved: {output_dir}/speedup_threads.png")


def plot_gflops_comparison(data, output_dir):
    """Bar chart comparing GFLOPS across methods for largest matrix."""
    sizes = sorted(set(int(r["matrix_size"]) for r in data))
    largest = sizes[-1]

    rows = [r for r in data if int(r["matrix_size"]) == largest
            and int(r["threads"]) <= 1
            or r["method"] in (f"combined_{max(sizes)}t",)]

    single_thread = [r for r in data
                     if int(r["matrix_size"]) == largest
                     and (int(r["threads"]) <= 1
                          or "combined" in r["method"])]

    methods = [r["method"] for r in single_thread]
    gflops = [float(r["gflops"]) for r in single_thread]

    fig, ax = plt.subplots(figsize=(12, 6))
    bars = ax.bar(range(len(methods)), gflops, color="steelblue")
    ax.set_xticks(range(len(methods)))
    ax.set_xticklabels(methods, rotation=45, ha="right")
    ax.set_ylabel("GFLOPS")
    ax.set_title(f"Performance Comparison (N={largest})")
    ax.grid(True, alpha=0.3, axis="y")
    fig.tight_layout()
    fig.savefig(os.path.join(output_dir, "gflops_comparison.png"), dpi=150)
    plt.close(fig)
    print(f"  Saved: {output_dir}/gflops_comparison.png")


def main():
    """Entry point: load CSV and generate all plots."""
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <benchmark.csv>")
        sys.exit(1)

    csv_path = sys.argv[1]
    output_dir = os.path.dirname(csv_path) or "results"
    os.makedirs(output_dir, exist_ok=True)

    data = load_csv(csv_path)
    print(f"Loaded {len(data)} rows from {csv_path}")

    plot_speedup_by_threads(data, output_dir)
    plot_gflops_comparison(data, output_dir)
    print("All plots generated.")


if __name__ == "__main__":
    main()
