#ifndef MAX_COVERAGE_H
#define MAX_COVERAGE_H

#include <vector>
#include <unordered_set>
#include <set>

/**
 * @brief Result of a maximum coverage algorithm
 */
struct CoverageResult {
    std::vector<int> selected_users;  // Indices of selected users
    int coverage;                      // Total unique locations covered
    double runtime_ms;                 // Runtime in milliseconds
};

/**
 * @brief Represents a user with their visited locations
 */
class User {
public:
    int id;
    std::unordered_set<int> locations;

    User(int user_id) : id(user_id) {}

    void add_location(int location_id) {
        locations.insert(location_id);
    }

    int num_locations() const {
        return locations.size();
    }
};

/**
 * @brief Greedy algorithm for maximum coverage problem
 *
 * Given n users, each with a set of visited locations, select k users
 * to maximize the total number of unique locations covered.
 *
 * Time Complexity: O(k * n * m) where m is avg locations per user
 * Approximation: (1 - 1/e) ≈ 0.632 of optimal
 *
 * @param users Vector of users with their location sets
 * @param k Maximum number of users to select
 * @return CoverageResult containing selected users and coverage
 */
CoverageResult greedy_max_coverage(const std::vector<User>& users, int k);

/**
 * @brief Brute force algorithm for maximum coverage (optimal solution)
 *
 * Tries all possible combinations of k users and returns the best.
 * Only feasible for small k (k <= 15) and small n (n <= 20).
 *
 * Time Complexity: O(C(n,k) * k * m) = O((n choose k) * k * m)
 *
 * @param users Vector of users with their location sets
 * @param k Number of users to select
 * @return CoverageResult containing optimal selection
 */
CoverageResult brute_force_max_coverage(const std::vector<User>& users, int k);

/**
 * @brief Random selection baseline
 *
 * Randomly selects k users and computes their coverage.
 * Used as a baseline to show greedy improvement.
 *
 * @param users Vector of users with their location sets
 * @param k Number of users to select
 * @param seed Random seed for reproducibility
 * @return CoverageResult containing random selection
 */
CoverageResult random_max_coverage(const std::vector<User>& users, int k, int seed = 42);

/**
 * @brief Compute coverage of a given set of users
 *
 * Helper function to compute total unique locations covered by a set of users.
 *
 * @param users All users
 * @param selected_indices Indices of selected users
 * @return Number of unique locations covered
 */
int compute_coverage(const std::vector<User>& users,
                     const std::vector<int>& selected_indices);

#endif // MAX_COVERAGE_H
