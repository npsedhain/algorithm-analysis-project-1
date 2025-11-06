#!/usr/bin/env python3
"""
Generate plots for greedy maximum coverage experiments
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Set style for publication-quality plots
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12
plt.rcParams['lines.linewidth'] = 2

OUTPUT_DIR = 'experiments/plots'
DATA_DIR = 'experiments/data'

def ensure_output_dir():
    """Create output directory if it doesn't exist"""
    os.makedirs(OUTPUT_DIR, exist_ok=True)

def plot_runtime_vs_n():
    """Plot 1: Runtime vs n (scalability)"""
    print("Generating plot: Runtime vs n...")

    df = pd.read_csv(f'{DATA_DIR}/runtime_vs_n.csv')

    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot runtime with error bars
    ax.errorbar(df['n'], df['avg_runtime_ms'], yerr=df['std_runtime_ms'],
                marker='o', markersize=8, capsize=5, capthick=2,
                label='Greedy Algorithm', color='#2E86AB')

    # Fit linear model (should be O(k*n*m))
    coeffs = np.polyfit(df['n'], df['avg_runtime_ms'], 1)
    fit_line = np.poly1d(coeffs)
    ax.plot(df['n'], fit_line(df['n']), '--',
            label=f'Linear Fit: {coeffs[0]:.4f}n + {coeffs[1]:.2f}',
            color='#A23B72', linewidth=1.5)

    ax.set_xlabel('Number of Users (n)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Runtime (ms)', fontsize=14, fontweight='bold')
    ax.set_title('Greedy Maximum Coverage: Runtime Scalability',
                 fontsize=16, fontweight='bold')
    ax.legend(fontsize=12)
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/runtime_vs_n.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/runtime_vs_n.png")
    plt.close()

def plot_coverage_vs_k():
    """Plot 2: Coverage vs k (diminishing returns)"""
    print("Generating plot: Coverage vs k...")

    df = pd.read_csv(f'{DATA_DIR}/coverage_vs_k.csv')

    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot greedy and random coverage
    ax.plot(df['k'], df['greedy_coverage'], marker='o', markersize=8,
            label='Greedy Algorithm', color='#2E86AB')
    ax.plot(df['k'], df['random_coverage'], marker='s', markersize=8,
            label='Random Selection', color='#F18F01')

    # Add improvement percentage
    improvement = ((df['greedy_coverage'] - df['random_coverage']) /
                   df['random_coverage'] * 100)

    ax.set_xlabel('Number of Friends Selected (k)', fontsize=14, fontweight='bold')
    ax.set_ylabel('Unique Locations Discovered', fontsize=14, fontweight='bold')
    ax.set_title('Location Coverage: Greedy vs Random Selection',
                 fontsize=16, fontweight='bold')
    ax.legend(fontsize=12)
    ax.grid(True, alpha=0.3)

    # Add text annotation for average improvement
    avg_improvement = improvement.mean()
    ax.text(0.98, 0.02, f'Avg Improvement: {avg_improvement:.1f}%',
            transform=ax.transAxes, fontsize=12,
            verticalalignment='bottom', horizontalalignment='right',
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/coverage_vs_k.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/coverage_vs_k.png")
    plt.close()

def plot_approximation_ratio():
    """Plot 3: Approximation ratio (greedy vs optimal)"""
    print("Generating plot: Approximation ratio...")

    df = pd.read_csv(f'{DATA_DIR}/approximation_ratio.csv')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Plot 1: Approximation ratio
    x_labels = [f'n={row["n"]}\nk={row["k"]}' for _, row in df.iterrows()]
    x_pos = np.arange(len(x_labels))

    bars = ax1.bar(x_pos, df['ratio'], color='#2E86AB', alpha=0.7, edgecolor='black')

    # Add theoretical guarantee line
    theoretical = (1 - 1/np.e)
    ax1.axhline(y=theoretical, color='#A23B72', linestyle='--', linewidth=2,
                label=f'Theoretical Guarantee: {theoretical:.3f}')

    # Color bars based on performance
    for i, bar in enumerate(bars):
        if df.iloc[i]['ratio'] >= theoretical:
            bar.set_color('#06A77D')  # Green if above guarantee

    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(x_labels, fontsize=10)
    ax1.set_ylabel('Approximation Ratio (Greedy/Optimal)', fontsize=12, fontweight='bold')
    ax1.set_title('Approximation Ratio vs Problem Size', fontsize=14, fontweight='bold')
    ax1.set_ylim([0.5, 1.05])
    ax1.legend(fontsize=11)
    ax1.grid(True, alpha=0.3, axis='y')

    # Plot 2: Runtime comparison
    width = 0.35
    ax2.bar(x_pos - width/2, df['greedy_time_ms'], width,
            label='Greedy', color='#2E86AB', alpha=0.7)
    ax2.bar(x_pos + width/2, df['optimal_time_ms'], width,
            label='Optimal (Brute Force)', color='#F18F01', alpha=0.7)

    ax2.set_xticks(x_pos)
    ax2.set_xticklabels(x_labels, fontsize=10)
    ax2.set_ylabel('Runtime (ms)', fontsize=12, fontweight='bold')
    ax2.set_title('Runtime Comparison: Greedy vs Optimal', fontsize=14, fontweight='bold')
    ax2.set_yscale('log')
    ax2.legend(fontsize=11)
    ax2.grid(True, alpha=0.3, axis='y')

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/approximation_ratio.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/approximation_ratio.png")
    plt.close()

def plot_zipf_comparison():
    """Plot 4: Performance on realistic (Zipf) data"""
    print("Generating plot: Zipf distribution results...")

    df = pd.read_csv(f'{DATA_DIR}/zipf_distribution.csv')

    fig, ax = plt.subplots(figsize=(12, 6))

    # Group by k value
    df['label'] = df.apply(lambda row: f'n={row["n"]}, k={row["k"]}', axis=1)

    x_pos = np.arange(len(df))
    width = 0.35

    bars1 = ax.bar(x_pos - width/2, df['greedy_coverage'], width,
                   label='Greedy', color='#2E86AB', alpha=0.7)
    bars2 = ax.bar(x_pos + width/2, df['random_coverage'], width,
                   label='Random', color='#F18F01', alpha=0.7)

    ax.set_xticks(x_pos)
    ax.set_xticklabels(df['label'], rotation=45, ha='right', fontsize=10)
    ax.set_ylabel('Unique Locations Discovered', fontsize=12, fontweight='bold')
    ax.set_title('Performance on Realistic Data (Zipf Distribution)',
                 fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3, axis='y')

    # Add percentage labels on bars
    for i, (bar1, bar2) in enumerate(zip(bars1, bars2)):
        improvement = ((df.iloc[i]['greedy_coverage'] -
                       df.iloc[i]['random_coverage']) /
                      df.iloc[i]['random_coverage'] * 100)
        ax.text(bar1.get_x() + bar1.get_width()/2, bar1.get_height() + 10,
                f'+{improvement:.0f}%', ha='center', va='bottom', fontsize=9,
                fontweight='bold', color='#06A77D')

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/zipf_distribution.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/zipf_distribution.png")
    plt.close()

def generate_summary_statistics():
    """Generate summary statistics for the paper"""
    print("\n" + "="*60)
    print("SUMMARY STATISTICS")
    print("="*60)

    # Runtime scaling
    df_runtime = pd.read_csv(f'{DATA_DIR}/runtime_vs_n.csv')
    print("\n1. Runtime Scalability (k=20):")
    print(f"   n=100:    {df_runtime.iloc[0]['avg_runtime_ms']:.2f} ms")
    print(f"   n=10000:  {df_runtime.iloc[-1]['avg_runtime_ms']:.2f} ms")
    print(f"   Linear fit coefficient: {np.polyfit(df_runtime['n'], df_runtime['avg_runtime_ms'], 1)[0]:.6f}")

    # Approximation ratio
    df_approx = pd.read_csv(f'{DATA_DIR}/approximation_ratio.csv')
    print("\n2. Approximation Ratio:")
    print(f"   Mean:   {df_approx['ratio'].mean():.4f}")
    print(f"   Min:    {df_approx['ratio'].min():.4f}")
    print(f"   Max:    {df_approx['ratio'].max():.4f}")
    print(f"   Theory: {1 - 1/np.e:.4f}")
    print(f"   All instances above guarantee: {(df_approx['ratio'] >= (1 - 1/np.e)).all()}")

    # Coverage improvement
    df_coverage = pd.read_csv(f'{DATA_DIR}/coverage_vs_k.csv')
    improvement = ((df_coverage['greedy_coverage'] - df_coverage['random_coverage']) /
                   df_coverage['random_coverage'] * 100)
    print("\n3. Improvement over Random:")
    print(f"   Mean:   {improvement.mean():.1f}%")
    print(f"   Min:    {improvement.min():.1f}%")
    print(f"   Max:    {improvement.max():.1f}%")

    print("\n" + "="*60)

# ===============================================
# DIVIDE AND CONQUER: CLOSEST PAIR PLOTS
# ===============================================

def plot_closest_pair_runtime():
    """Plot: Closest Pair Runtime Comparison (O(n log n) vs O(n²))"""
    print("Generating plot: Closest Pair Runtime...")

    df = pd.read_csv(f'{DATA_DIR}/closest_pair_runtime.csv')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Left plot: Absolute runtime comparison
    mask = df['bf_runtime_ms'] > 0  # Only where brute force was run
    ax1.errorbar(df.loc[mask, 'n'], df.loc[mask, 'dc_runtime_ms'],
                yerr=df.loc[mask, 'dc_std_ms'],
                marker='o', markersize=8, capsize=5, capthick=2,
                label='Divide & Conquer O(n log n)', color='#2E86AB')
    ax1.errorbar(df.loc[mask, 'n'], df.loc[mask, 'bf_runtime_ms'],
                yerr=df.loc[mask, 'bf_std_ms'],
                marker='s', markersize=8, capsize=5, capthick=2,
                label='Brute Force O(n²)', color='#F18F01')

    ax1.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Runtime (ms)', fontsize=14, fontweight='bold')
    ax1.set_title('Closest Pair: Runtime Comparison', fontsize=16, fontweight='bold')
    ax1.legend(fontsize=12)
    ax1.grid(True, alpha=0.3)

    # Right plot: Log scale to show exponential difference
    ax2.semilogy(df.loc[mask, 'n'], df.loc[mask, 'dc_runtime_ms'],
                marker='o', markersize=8, label='Divide & Conquer', color='#2E86AB')
    ax2.semilogy(df.loc[mask, 'n'], df.loc[mask, 'bf_runtime_ms'],
                marker='s', markersize=8, label='Brute Force', color='#F18F01')

    # Add all D&C points including large n
    ax2.plot(df['n'], df['dc_runtime_ms'], 'o-', markersize=6,
             label='D&C (all sizes)', color='#06A77D', alpha=0.7)

    ax2.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Runtime (ms, log scale)', fontsize=14, fontweight='bold')
    ax2.set_title('Runtime Comparison (Log Scale)', fontsize=16, fontweight='bold')
    ax2.legend(fontsize=12)
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/closest_pair_runtime.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/closest_pair_runtime.png")
    plt.close()

def plot_closest_pair_complexity():
    """Plot: Verify O(n log n) complexity"""
    print("Generating plot: Complexity Verification...")

    df = pd.read_csv(f'{DATA_DIR}/closest_pair_complexity.csv')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Left plot: Runtime vs n (actual data with O(n log n) fit)
    ax1.plot(df['n'], df['runtime_ms'], 'o-', markersize=6,
            label='Actual Runtime', color='#2E86AB', linewidth=2)

    # Fit to O(n log n)
    # Runtime = c * n * log(n)
    # Find constant c
    c = (df['runtime_ms'] / df['n_log_n']).mean()
    ax1.plot(df['n'], c * df['n_log_n'], '--',
            label=f'O(n log n) fit: {c:.4f}·n log n', color='#A23B72', linewidth=1.5)

    ax1.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Runtime (ms)', fontsize=14, fontweight='bold')
    ax1.set_title('Divide & Conquer: O(n log n) Verification', fontsize=16, fontweight='bold')
    ax1.legend(fontsize=12)
    ax1.grid(True, alpha=0.3)

    # Right plot: Normalized runtime (runtime / n log n should be constant)
    normalized = df['runtime_ms'] / df['n_log_n']
    ax2.plot(df['n'], normalized, 'o-', markersize=6, color='#06A77D', linewidth=2)
    ax2.axhline(y=normalized.mean(), color='#A23B72', linestyle='--', linewidth=1.5,
               label=f'Mean: {normalized.mean():.6f}')

    ax2.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Runtime / (n log n)', fontsize=14, fontweight='bold')
    ax2.set_title('Normalized Runtime (Should be Constant)', fontsize=16, fontweight='bold')
    ax2.legend(fontsize=12)
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/closest_pair_complexity.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/closest_pair_complexity.png")
    plt.close()

def plot_closest_pair_distributions():
    """Plot: Performance on different data distributions"""
    print("Generating plot: Data Distributions...")

    df = pd.read_csv(f'{DATA_DIR}/closest_pair_distributions.csv')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Left plot: Runtime comparison
    for dist in df['distribution'].unique():
        data = df[df['distribution'] == dist]
        ax1.plot(data['n'], data['runtime_ms'], 'o-', markersize=8,
                label=dist.capitalize(), linewidth=2)

    ax1.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Runtime (ms)', fontsize=14, fontweight='bold')
    ax1.set_title('Runtime on Different Data Distributions', fontsize=16, fontweight='bold')
    ax1.legend(fontsize=12)
    ax1.grid(True, alpha=0.3)

    # Right plot: Comparisons made
    for dist in df['distribution'].unique():
        data = df[df['distribution'] == dist]
        ax2.plot(data['n'], data['comparisons'], 's-', markersize=8,
                label=dist.capitalize(), linewidth=2)

    ax2.set_xlabel('Number of Points (n)', fontsize=14, fontweight='bold')
    ax2.set_ylabel('Number of Distance Comparisons', fontsize=14, fontweight='bold')
    ax2.set_title('Comparisons on Different Distributions', fontsize=16, fontweight='bold')
    ax2.legend(fontsize=12)
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/closest_pair_distributions.png', dpi=300, bbox_inches='tight')
    print(f"  Saved to {OUTPUT_DIR}/closest_pair_distributions.png")
    plt.close()

def generate_closest_pair_statistics():
    """Generate summary statistics for closest pair experiments"""
    print("\n" + "="*60)
    print("CLOSEST PAIR SUMMARY STATISTICS")
    print("="*60)

    # Runtime comparison
    df_runtime = pd.read_csv(f'{DATA_DIR}/closest_pair_runtime.csv')
    print("\n1. Runtime Comparison (n=5000):")
    row = df_runtime[df_runtime['n'] == 5000].iloc[0]
    print(f"   Divide & Conquer: {row['dc_runtime_ms']:.2f} ms")
    print(f"   Brute Force:      {row['bf_runtime_ms']:.2f} ms")
    print(f"   Speedup:          {row['bf_runtime_ms']/row['dc_runtime_ms']:.1f}x")

    # Complexity verification
    df_complex = pd.read_csv(f'{DATA_DIR}/closest_pair_complexity.csv')
    normalized = df_complex['runtime_ms'] / df_complex['n_log_n']
    print("\n2. Complexity Verification:")
    print(f"   Runtime / (n log n) mean:   {normalized.mean():.6f}")
    print(f"   Runtime / (n log n) std:    {normalized.std():.6f}")
    print(f"   Coefficient of variation:   {(normalized.std()/normalized.mean()*100):.2f}%")
    print(f"   Confirms O(n log n): {'YES' if normalized.std()/normalized.mean() < 0.3 else 'NO'}")

    # Largest instance
    largest = df_complex.iloc[-1]
    print("\n3. Largest Instance:")
    print(f"   n = {int(largest['n'])}")
    print(f"   Runtime: {largest['runtime_ms']:.2f} ms")
    print(f"   Comparisons: {int(largest['comparisons'])}")

    print("\n" + "="*60)

def main():
    """Generate all plots"""
    print("="*60)
    print("Algorithm Analysis - Plot Generation")
    print("="*60)

    ensure_output_dir()

    # Generate greedy algorithm plots
    print("\n--- GREEDY ALGORITHM PLOTS ---\n")
    plot_runtime_vs_n()
    plot_coverage_vs_k()
    plot_approximation_ratio()
    plot_zipf_comparison()

    # Generate closest pair plots
    print("\n--- CLOSEST PAIR PLOTS ---\n")
    plot_closest_pair_runtime()
    plot_closest_pair_complexity()
    plot_closest_pair_distributions()

    # Generate summaries
    generate_summary_statistics()
    generate_closest_pair_statistics()

    print("\n" + "="*60)
    print("All plots generated successfully!")
    print(f"Output directory: {OUTPUT_DIR}/")
    print("="*60)

if __name__ == '__main__':
    main()
