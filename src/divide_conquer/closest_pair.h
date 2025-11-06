#ifndef CLOSEST_PAIR_H
#define CLOSEST_PAIR_H

#include <vector>
#include <cmath>
#include <utility>

/**
 * @brief Represents a point in 2D space (user location)
 */
struct Point {
    double x, y;
    int id;  // User ID for tracking

    Point(double x_coord = 0, double y_coord = 0, int user_id = 0)
        : x(x_coord), y(y_coord), id(user_id) {}
};

/**
 * @brief Result of closest pair algorithm
 */
struct ClosestPairResult {
    Point p1, p2;           // The two closest points
    double distance;        // Distance between them
    double runtime_ms;      // Runtime in milliseconds
    int comparisons;        // Number of distance comparisons made
};

/**
 * @brief Compute Euclidean distance between two points
 *
 * @param p1 First point
 * @param p2 Second point
 * @return Euclidean distance
 */
inline double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

/**
 * @brief Divide and conquer algorithm for closest pair of points
 *
 * Classic computational geometry problem: find two points with minimum distance.
 *
 * Algorithm:
 * 1. Sort points by x-coordinate
 * 2. Divide: Split into left and right halves
 * 3. Conquer: Recursively find closest pair in each half
 * 4. Combine: Check pairs across the dividing line
 * 5. Only check points within strip of width 2*delta
 *
 * Time Complexity: O(n log n)
 * - Initial sort: O(n log n)
 * - Recurrence: T(n) = 2T(n/2) + O(n)
 * - Master theorem: T(n) = O(n log n)
 *
 * Space Complexity: O(n) for auxiliary arrays
 *
 * @param points Vector of 2D points (will be modified - sorted)
 * @return ClosestPairResult containing the closest pair
 */
ClosestPairResult divide_conquer_closest_pair(std::vector<Point>& points);

/**
 * @brief Brute force algorithm for closest pair (O(n²))
 *
 * Checks all pairs of points and returns the closest.
 * Used for comparison and validation of divide-and-conquer.
 *
 * Time Complexity: O(n²)
 *
 * @param points Vector of 2D points
 * @return ClosestPairResult containing the closest pair
 */
ClosestPairResult brute_force_closest_pair(const std::vector<Point>& points);

#endif // CLOSEST_PAIR_H
