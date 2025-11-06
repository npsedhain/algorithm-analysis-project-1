#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "../greedy/max_coverage.h"
#include <vector>
#include <random>

/**
 * @brief Generate synthetic location-based social network data
 */
class DataGenerator {
private:
    std::mt19937 rng;

public:
    /**
     * @brief Constructor with random seed
     * @param seed Random seed for reproducibility
     */
    DataGenerator(int seed = 42) : rng(seed) {}

    /**
     * @brief Generate users with random location sets (uniform distribution)
     *
     * Each user visits approximately avg_locations locations,
     * sampled uniformly from the pool of total_locations.
     *
     * @param n_users Number of users to generate
     * @param total_locations Total number of locations in the pool
     * @param avg_locations Average number of locations per user
     * @param variance Variance in locations per user (default 0.2)
     * @return Vector of users with randomly assigned locations
     */
    std::vector<User> generate_uniform(int n_users,
                                       int total_locations,
                                       int avg_locations,
                                       double variance = 0.2) {
        std::vector<User> users;
        users.reserve(n_users);

        std::uniform_int_distribution<> location_dist(0, total_locations - 1);

        for (int i = 0; i < n_users; ++i) {
            User user(i);

            // Determine number of locations for this user (with variance)
            int num_locs = sample_normal(avg_locations, avg_locations * variance);
            num_locs = std::max(1, std::min(num_locs, total_locations));

            // Sample random locations (with replacement, then deduplicate via set)
            for (int j = 0; j < num_locs * 2; ++j) {  // Sample extra to account for duplicates
                int loc = location_dist(rng);
                user.add_location(loc);
                if (user.num_locations() >= num_locs) break;
            }

            users.push_back(user);
        }

        return users;
    }

    /**
     * @brief Generate users with Zipf-distributed location popularity
     *
     * Some locations are visited by many users (popular places),
     * while others are visited by few (niche places).
     *
     * @param n_users Number of users to generate
     * @param total_locations Total number of locations
     * @param avg_locations Average locations per user
     * @param alpha Zipf parameter (higher = more skewed, default 1.0)
     * @return Vector of users with Zipf-distributed locations
     */
    std::vector<User> generate_zipf(int n_users,
                                    int total_locations,
                                    int avg_locations,
                                    double alpha = 1.0) {
        std::vector<User> users;
        users.reserve(n_users);

        // Generate Zipf probabilities
        std::vector<double> probabilities = generate_zipf_probabilities(total_locations, alpha);
        std::discrete_distribution<> location_dist(probabilities.begin(), probabilities.end());

        for (int i = 0; i < n_users; ++i) {
            User user(i);

            int num_locs = sample_normal(avg_locations, avg_locations * 0.2);
            num_locs = std::max(1, std::min(num_locs, total_locations));

            // Sample locations according to Zipf distribution
            for (int j = 0; j < num_locs * 2; ++j) {
                int loc = location_dist(rng);
                user.add_location(loc);
                if (user.num_locations() >= num_locs) break;
            }

            users.push_back(user);
        }

        return users;
    }

private:
    /**
     * @brief Sample from normal distribution
     */
    int sample_normal(double mean, double stddev) {
        std::normal_distribution<> dist(mean, stddev);
        return static_cast<int>(std::round(dist(rng)));
    }

    /**
     * @brief Generate Zipf distribution probabilities
     *
     * P(k) ∝ 1 / k^alpha
     */
    std::vector<double> generate_zipf_probabilities(int n, double alpha) {
        std::vector<double> probs(n);
        double sum = 0.0;

        for (int k = 1; k <= n; ++k) {
            probs[k - 1] = 1.0 / std::pow(k, alpha);
            sum += probs[k - 1];
        }

        // Normalize
        for (double& p : probs) {
            p /= sum;
        }

        return probs;
    }
};

#endif // DATA_GENERATOR_H
