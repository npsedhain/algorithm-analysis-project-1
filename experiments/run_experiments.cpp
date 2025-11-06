#include "../src/greedy/max_coverage.h"
#include "../src/divide_conquer/closest_pair.h"
#include "../src/common/data_generator.h"
#include "../src/common/timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>

/**
 * @brief Run experiments to validate greedy algorithm
 *
 * Generates CSV files with experimental results for:
 * 1. Runtime vs n (varying number of users)
 * 2. Coverage vs k (varying selection size)
 * 3. Approximation ratio (greedy vs optimal)
 * 4. Comparison with random baseline
 */

void print_header() {
    std::cout << "========================================\n";
    std::cout << "Maximum Coverage Greedy Algorithm\n";
    std::cout << "Experimental Validation\n";
    std::cout << "========================================\n\n";
}

/**
 * @brief Experiment 1: Runtime vs n (scalability test)
 */
void experiment_runtime_vs_n(const std::string& output_file) {
    std::cout << "Experiment 1: Runtime vs n...\n";

    std::ofstream out(output_file);
    out << "n,k,avg_runtime_ms,std_runtime_ms,coverage\n";

    std::vector<int> n_values = {100, 200, 500, 1000, 2000, 5000, 10000};
    int k = 20;
    int total_locations = 5000;
    int avg_locations = 50;
    int trials = 10;

    DataGenerator gen(42);

    for (int n : n_values) {
        std::cout << "  n = " << n << "..." << std::flush;

        std::vector<double> runtimes;
        int total_coverage = 0;

        for (int trial = 0; trial < trials; ++trial) {
            auto users = gen.generate_uniform(n, total_locations, avg_locations);
            auto result = greedy_max_coverage(users, k);
            runtimes.push_back(result.runtime_ms);
            total_coverage += result.coverage;
        }

        // Compute statistics
        double mean_runtime = 0.0;
        for (double rt : runtimes) mean_runtime += rt;
        mean_runtime /= trials;

        double std_runtime = 0.0;
        for (double rt : runtimes) {
            std_runtime += (rt - mean_runtime) * (rt - mean_runtime);
        }
        std_runtime = std::sqrt(std_runtime / trials);

        double avg_coverage = total_coverage / (double)trials;

        out << n << "," << k << "," << mean_runtime << ","
            << std_runtime << "," << avg_coverage << "\n";

        std::cout << " done (avg: " << mean_runtime << " ms)\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

/**
 * @brief Experiment 2: Coverage vs k
 */
void experiment_coverage_vs_k(const std::string& output_file) {
    std::cout << "Experiment 2: Coverage vs k...\n";

    std::ofstream out(output_file);
    out << "k,greedy_coverage,random_coverage,greedy_runtime_ms,random_runtime_ms\n";

    int n = 1000;
    int total_locations = 5000;
    int avg_locations = 50;
    int trials = 10;
    std::vector<int> k_values = {5, 10, 15, 20, 30, 50, 75, 100};

    DataGenerator gen(42);

    for (int k : k_values) {
        std::cout << "  k = " << k << "..." << std::flush;

        double greedy_cov = 0.0, random_cov = 0.0;
        double greedy_time = 0.0, random_time = 0.0;

        for (int trial = 0; trial < trials; ++trial) {
            auto users = gen.generate_uniform(n, total_locations, avg_locations);

            auto greedy_result = greedy_max_coverage(users, k);
            auto random_result = random_max_coverage(users, k, trial);

            greedy_cov += greedy_result.coverage;
            random_cov += random_result.coverage;
            greedy_time += greedy_result.runtime_ms;
            random_time += random_result.runtime_ms;
        }

        greedy_cov /= trials;
        random_cov /= trials;
        greedy_time /= trials;
        random_time /= trials;

        out << k << "," << greedy_cov << "," << random_cov << ","
            << greedy_time << "," << random_time << "\n";

        std::cout << " done (greedy: " << greedy_cov
                  << ", random: " << random_cov << ")\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

/**
 * @brief Experiment 3: Approximation ratio (greedy vs optimal)
 */
void experiment_approximation_ratio(const std::string& output_file) {
    std::cout << "Experiment 3: Approximation ratio (greedy vs optimal)...\n";
    std::cout << "  Note: Using small n and k for brute force feasibility\n";

    std::ofstream out(output_file);
    out << "n,k,greedy_coverage,optimal_coverage,ratio,greedy_time_ms,optimal_time_ms\n";

    int total_locations = 100;
    int avg_locations = 20;
    int trials = 5;

    DataGenerator gen(42);

    // Small instances only (brute force is exponential)
    std::vector<std::pair<int, int>> configs = {
        {10, 3}, {10, 5}, {12, 4}, {15, 5}, {15, 7}, {18, 5}, {20, 5}
    };

    for (auto [n, k] : configs) {
        std::cout << "  n = " << n << ", k = " << k << "..." << std::flush;

        double greedy_cov = 0.0, optimal_cov = 0.0;
        double greedy_time = 0.0, optimal_time = 0.0;
        double ratio_sum = 0.0;

        for (int trial = 0; trial < trials; ++trial) {
            auto users = gen.generate_uniform(n, total_locations, avg_locations);

            auto greedy_result = greedy_max_coverage(users, k);
            auto optimal_result = brute_force_max_coverage(users, k);

            greedy_cov += greedy_result.coverage;
            optimal_cov += optimal_result.coverage;
            greedy_time += greedy_result.runtime_ms;
            optimal_time += optimal_result.runtime_ms;

            double ratio = optimal_result.coverage > 0 ?
                (double)greedy_result.coverage / optimal_result.coverage : 1.0;
            ratio_sum += ratio;
        }

        greedy_cov /= trials;
        optimal_cov /= trials;
        greedy_time /= trials;
        optimal_time /= trials;
        double avg_ratio = ratio_sum / trials;

        out << n << "," << k << "," << greedy_cov << "," << optimal_cov << ","
            << avg_ratio << "," << greedy_time << "," << optimal_time << "\n";

        std::cout << " ratio = " << std::fixed << std::setprecision(3)
                  << avg_ratio << "\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

/**
 * @brief Experiment 4: Zipf distribution (realistic data)
 */
void experiment_zipf_distribution(const std::string& output_file) {
    std::cout << "Experiment 4: Zipf distribution (realistic popularity)...\n";

    std::ofstream out(output_file);
    out << "n,k,greedy_coverage,random_coverage,greedy_time_ms\n";

    int total_locations = 5000;
    int avg_locations = 50;
    double alpha = 1.0;  // Zipf parameter
    int trials = 10;

    std::vector<std::pair<int, int>> configs = {
        {500, 10}, {1000, 10}, {2000, 10}, {5000, 10},
        {1000, 20}, {1000, 50}
    };

    DataGenerator gen(42);

    for (auto [n, k] : configs) {
        std::cout << "  n = " << n << ", k = " << k << "..." << std::flush;

        double greedy_cov = 0.0, random_cov = 0.0, greedy_time = 0.0;

        for (int trial = 0; trial < trials; ++trial) {
            auto users = gen.generate_zipf(n, total_locations, avg_locations, alpha);

            auto greedy_result = greedy_max_coverage(users, k);
            auto random_result = random_max_coverage(users, k, trial);

            greedy_cov += greedy_result.coverage;
            random_cov += random_result.coverage;
            greedy_time += greedy_result.runtime_ms;
        }

        greedy_cov /= trials;
        random_cov /= trials;
        greedy_time /= trials;

        out << n << "," << k << "," << greedy_cov << ","
            << random_cov << "," << greedy_time << "\n";

        std::cout << " done (greedy: " << greedy_cov << ")\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

// ===============================================
// DIVIDE AND CONQUER: CLOSEST PAIR EXPERIMENTS
// ===============================================

/**
 * @brief Generate random points uniformly distributed in a square
 */
std::vector<Point> generate_uniform_points(int n, double min_coord, double max_coord, int seed = 42) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(min_coord, max_coord);

    std::vector<Point> points;
    points.reserve(n);

    for (int i = 0; i < n; ++i) {
        points.emplace_back(dist(gen), dist(gen), i);
    }

    return points;
}

/**
 * @brief Generate points in clusters (realistic social network scenario)
 */
std::vector<Point> generate_clustered_points(int n, int num_clusters, double cluster_radius, int seed = 42) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> center_dist(0.0, 1000.0);
    std::normal_distribution<double> cluster_dist(0.0, cluster_radius);

    std::vector<Point> points;
    points.reserve(n);

    // Generate cluster centers
    std::vector<std::pair<double, double>> centers;
    for (int i = 0; i < num_clusters; ++i) {
        centers.emplace_back(center_dist(gen), center_dist(gen));
    }

    // Assign points to clusters
    for (int i = 0; i < n; ++i) {
        int cluster_id = i % num_clusters;
        double x = centers[cluster_id].first + cluster_dist(gen);
        double y = centers[cluster_id].second + cluster_dist(gen);
        points.emplace_back(x, y, i);
    }

    return points;
}

/**
 * @brief Experiment 5: Closest Pair - Runtime vs n (scalability test)
 */
void experiment_closest_pair_runtime(const std::string& output_file) {
    std::cout << "Experiment 5: Closest Pair - Runtime vs n (O(n log n) vs O(n^2))...\n";

    std::ofstream out(output_file);
    out << "n,dc_runtime_ms,dc_std_ms,bf_runtime_ms,bf_std_ms,dc_comparisons,bf_comparisons,distance\n";

    // Test different problem sizes
    std::vector<int> n_values = {100, 200, 500, 1000, 2000, 5000};
    std::vector<int> n_values_large = {10000, 20000, 50000};

    int trials = 10;

    // Small to medium sizes (both algorithms)
    for (int n : n_values) {
        std::cout << "  n = " << n << "..." << std::flush;

        std::vector<double> dc_runtimes, bf_runtimes;
        int dc_comps = 0, bf_comps = 0;
        double min_dist = 0.0;

        for (int trial = 0; trial < trials; ++trial) {
            auto points = generate_uniform_points(n, 0.0, 1000.0, 42 + trial);

            // Divide and conquer
            auto points_dc = points;
            auto dc_result = divide_conquer_closest_pair(points_dc);
            dc_runtimes.push_back(dc_result.runtime_ms);
            dc_comps += dc_result.comparisons;

            // Brute force
            auto bf_result = brute_force_closest_pair(points);
            bf_runtimes.push_back(bf_result.runtime_ms);
            bf_comps += bf_result.comparisons;

            min_dist += dc_result.distance;
        }

        // Compute statistics
        double dc_mean = 0.0, bf_mean = 0.0;
        for (int i = 0; i < trials; ++i) {
            dc_mean += dc_runtimes[i];
            bf_mean += bf_runtimes[i];
        }
        dc_mean /= trials;
        bf_mean /= trials;

        double dc_std = 0.0, bf_std = 0.0;
        for (int i = 0; i < trials; ++i) {
            dc_std += (dc_runtimes[i] - dc_mean) * (dc_runtimes[i] - dc_mean);
            bf_std += (bf_runtimes[i] - bf_mean) * (bf_runtimes[i] - bf_mean);
        }
        dc_std = std::sqrt(dc_std / trials);
        bf_std = std::sqrt(bf_std / trials);

        out << n << "," << dc_mean << "," << dc_std << ","
            << bf_mean << "," << bf_std << ","
            << (dc_comps / trials) << "," << (bf_comps / trials) << ","
            << (min_dist / trials) << "\n";

        std::cout << " done (DC: " << dc_mean << " ms, BF: " << bf_mean << " ms)\n";
    }

    // Large sizes (only divide & conquer)
    for (int n : n_values_large) {
        std::cout << "  n = " << n << " (DC only)..." << std::flush;

        std::vector<double> dc_runtimes;
        int dc_comps = 0;
        double min_dist = 0.0;

        for (int trial = 0; trial < trials; ++trial) {
            auto points = generate_uniform_points(n, 0.0, 1000.0, 42 + trial);
            auto dc_result = divide_conquer_closest_pair(points);
            dc_runtimes.push_back(dc_result.runtime_ms);
            dc_comps += dc_result.comparisons;
            min_dist += dc_result.distance;
        }

        double dc_mean = 0.0;
        for (double rt : dc_runtimes) dc_mean += rt;
        dc_mean /= trials;

        double dc_std = 0.0;
        for (double rt : dc_runtimes) {
            dc_std += (rt - dc_mean) * (rt - dc_mean);
        }
        dc_std = std::sqrt(dc_std / trials);

        out << n << "," << dc_mean << "," << dc_std << ",-1,-1,"
            << (dc_comps / trials) << ",-1," << (min_dist / trials) << "\n";

        std::cout << " done (DC: " << dc_mean << " ms)\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

/**
 * @brief Experiment 6: Closest Pair - Different Data Distributions
 */
void experiment_closest_pair_distributions(const std::string& output_file) {
    std::cout << "Experiment 6: Closest Pair - Different Data Distributions...\n";

    std::ofstream out(output_file);
    out << "n,distribution,runtime_ms,comparisons,min_distance\n";

    std::vector<int> n_values = {1000, 5000, 10000};
    int trials = 10;

    for (int n : n_values) {
        std::cout << "  n = " << n << "...\n";

        // Uniform distribution
        {
            std::cout << "    Uniform..." << std::flush;
            double total_runtime = 0.0;
            int total_comps = 0;
            double total_dist = 0.0;

            for (int trial = 0; trial < trials; ++trial) {
                auto points = generate_uniform_points(n, 0.0, 1000.0, 42 + trial);
                auto result = divide_conquer_closest_pair(points);
                total_runtime += result.runtime_ms;
                total_comps += result.comparisons;
                total_dist += result.distance;
            }

            out << n << ",uniform," << (total_runtime / trials) << ","
                << (total_comps / trials) << "," << (total_dist / trials) << "\n";
            std::cout << " done\n";
        }

        // Clustered distribution
        {
            std::cout << "    Clustered..." << std::flush;
            double total_runtime = 0.0;
            int total_comps = 0;
            double total_dist = 0.0;

            for (int trial = 0; trial < trials; ++trial) {
                auto points = generate_clustered_points(n, 10, 20.0, 42 + trial);
                auto result = divide_conquer_closest_pair(points);
                total_runtime += result.runtime_ms;
                total_comps += result.comparisons;
                total_dist += result.distance;
            }

            out << n << ",clustered," << (total_runtime / trials) << ","
                << (total_comps / trials) << "," << (total_dist / trials) << "\n";
            std::cout << " done\n";
        }
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

/**
 * @brief Experiment 7: Closest Pair - Complexity Verification
 */
void experiment_closest_pair_complexity(const std::string& output_file) {
    std::cout << "Experiment 7: Closest Pair - Complexity Verification (O(n log n))...\n";

    std::ofstream out(output_file);
    out << "n,runtime_ms,comparisons,n_log_n,n_squared\n";

    std::vector<int> n_values;
    for (int n = 100; n <= 50000; n = (int)(n * 1.5)) {
        n_values.push_back(n);
    }

    int trials = 5;

    for (int n : n_values) {
        std::cout << "  n = " << n << "..." << std::flush;

        double total_runtime = 0.0;
        int total_comps = 0;

        for (int trial = 0; trial < trials; ++trial) {
            auto points = generate_uniform_points(n, 0.0, 1000.0, 42 + trial);
            auto result = divide_conquer_closest_pair(points);
            total_runtime += result.runtime_ms;
            total_comps += result.comparisons;
        }

        double avg_runtime = total_runtime / trials;
        int avg_comps = total_comps / trials;
        double n_log_n = n * std::log2(n);
        double n_squared = n * n;

        out << n << "," << avg_runtime << "," << avg_comps << ","
            << n_log_n << "," << n_squared << "\n";

        std::cout << " done (" << avg_runtime << " ms)\n";
    }

    out.close();
    std::cout << "  Results saved to " << output_file << "\n\n";
}

int main() {
    print_header();

    // Create output directory if it doesn't exist
    system("mkdir -p experiments/data");

    // Run greedy algorithm experiments
    std::cout << "\n===== GREEDY ALGORITHM EXPERIMENTS =====\n\n";
    experiment_runtime_vs_n("experiments/data/runtime_vs_n.csv");
    experiment_coverage_vs_k("experiments/data/coverage_vs_k.csv");
    experiment_approximation_ratio("experiments/data/approximation_ratio.csv");
    experiment_zipf_distribution("experiments/data/zipf_distribution.csv");

    // Run closest pair experiments
    std::cout << "\n===== DIVIDE & CONQUER EXPERIMENTS =====\n\n";
    experiment_closest_pair_runtime("experiments/data/closest_pair_runtime.csv");
    experiment_closest_pair_distributions("experiments/data/closest_pair_distributions.csv");
    experiment_closest_pair_complexity("experiments/data/closest_pair_complexity.csv");

    std::cout << "========================================\n";
    std::cout << "All experiments completed!\n";
    std::cout << "Results saved in experiments/data/\n";
    std::cout << "Run Python scripts to generate plots.\n";
    std::cout << "========================================\n";

    return 0;
}
