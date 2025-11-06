#include "max_coverage.h"
#include "../common/timer.h"
#include <algorithm>
#include <random>
#include <iostream>

// Helper function to compute coverage
int compute_coverage(const std::vector<User>& users,
                     const std::vector<int>& selected_indices) {
    std::unordered_set<int> covered;
    for (int idx : selected_indices) {
        for (int loc : users[idx].locations) {
            covered.insert(loc);
        }
    }
    return covered.size();
}

// Greedy maximum coverage algorithm
CoverageResult greedy_max_coverage(const std::vector<User>& users, int k) {
    Timer timer;
    timer.start();

    CoverageResult result;
    result.selected_users.reserve(k);

    std::unordered_set<int> covered;  // Locations covered so far
    std::vector<bool> selected(users.size(), false);  // Track selected users

    // Greedy selection: k iterations
    for (int iteration = 0; iteration < k && iteration < users.size(); ++iteration) {
        int best_user = -1;
        int max_gain = 0;

        // Find user with maximum marginal gain
        for (size_t u = 0; u < users.size(); ++u) {
            if (selected[u]) continue;  // Skip already selected users

            // Compute marginal gain: count new locations
            int gain = 0;
            for (int loc : users[u].locations) {
                if (covered.find(loc) == covered.end()) {
                    gain++;
                }
            }

            // Update best user if this gain is better
            if (gain > max_gain) {
                max_gain = gain;
                best_user = u;
            }
        }

        // If no user provides positive gain, stop early
        if (best_user == -1 || max_gain == 0) {
            break;
        }

        // Select the best user
        selected[best_user] = true;
        result.selected_users.push_back(best_user);

        // Update covered locations
        for (int loc : users[best_user].locations) {
            covered.insert(loc);
        }
    }

    result.coverage = covered.size();
    result.runtime_ms = timer.elapsed_ms();

    return result;
}

// Brute force algorithm (optimal solution for small inputs)
CoverageResult brute_force_max_coverage(const std::vector<User>& users, int k) {
    Timer timer;
    timer.start();

    CoverageResult result;
    result.coverage = 0;

    int n = users.size();
    if (k > n) k = n;
    if (n > 20 || k > 15) {
        std::cerr << "Warning: Brute force not feasible for n=" << n << ", k=" << k << std::endl;
        result.runtime_ms = -1;
        return result;
    }

    // Generate all combinations of k users
    std::vector<int> combination(k);
    std::function<void(int, int)> generate_combinations = [&](int start, int depth) {
        if (depth == k) {
            // Compute coverage for this combination
            int cov = compute_coverage(users, combination);
            if (cov > result.coverage) {
                result.coverage = cov;
                result.selected_users = combination;
            }
            return;
        }

        for (int i = start; i < n; ++i) {
            combination[depth] = i;
            generate_combinations(i + 1, depth + 1);
        }
    };

    generate_combinations(0, 0);
    result.runtime_ms = timer.elapsed_ms();

    return result;
}

// Random selection baseline
CoverageResult random_max_coverage(const std::vector<User>& users, int k, int seed) {
    Timer timer;
    timer.start();

    CoverageResult result;

    int n = users.size();
    if (k > n) k = n;

    // Create random permutation of user indices
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i) {
        indices[i] = i;
    }

    std::mt19937 rng(seed);
    std::shuffle(indices.begin(), indices.end(), rng);

    // Select first k users from shuffled list
    result.selected_users.assign(indices.begin(), indices.begin() + k);

    // Compute coverage
    result.coverage = compute_coverage(users, result.selected_users);
    result.runtime_ms = timer.elapsed_ms();

    return result;
}
