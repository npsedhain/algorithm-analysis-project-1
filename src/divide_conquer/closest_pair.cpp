#include "divide_conquer/closest_pair.h"
#include "common/timer.h"
#include <algorithm>
#include <limits>
#include <vector>

// Comparators for sorting
bool compare_x(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool compare_y(const Point& a, const Point& b) {
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

/**
 * @brief Brute force for small instances (n <= 3)
 *
 * Base case for divide and conquer recursion.
 * Also used for validation against O(n²) baseline.
 */
ClosestPairResult brute_force_closest_pair_impl(const std::vector<Point>& points, int& comparisons) {
    double min_dist = std::numeric_limits<double>::infinity();
    Point p1, p2;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            comparisons++;
            double dist = distance(points[i], points[j]);
            if (dist < min_dist) {
                min_dist = dist;
                p1 = points[i];
                p2 = points[j];
            }
        }
    }

    ClosestPairResult result;
    result.p1 = p1;
    result.p2 = p2;
    result.distance = min_dist;
    result.comparisons = 0;  // Will be set by caller
    return result;
}

/**
 * @brief Find closest pair in a vertical strip
 *
 * After dividing into left and right halves, check points near the dividing line.
 * Only need to check points within distance delta of the line.
 *
 * Key optimization: For each point, only check next 7 points in y-sorted order.
 * Proof: In a 2*delta x delta rectangle, at most 8 points can fit with min distance > delta.
 *
 * @param strip Points in the strip, sorted by y-coordinate
 * @param delta Current minimum distance
 * @param comparisons Counter for number of distance comparisons
 * @return Minimum distance and corresponding pair in the strip
 */
ClosestPairResult find_strip_closest(std::vector<Point>& strip, double delta, int& comparisons) {
    double min_dist = delta;
    Point p1, p2;
    int n = strip.size();

    // Sort strip by y-coordinate
    std::sort(strip.begin(), strip.end(), compare_y);

    // For each point, only check next 7 points (proven sufficient)
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n && (strip[j].y - strip[i].y) < min_dist; ++j) {
            comparisons++;
            double dist = distance(strip[i], strip[j]);
            if (dist < min_dist) {
                min_dist = dist;
                p1 = strip[i];
                p2 = strip[j];
            }
        }
    }

    ClosestPairResult result;
    result.p1 = p1;
    result.p2 = p2;
    result.distance = min_dist;
    return result;
}

/**
 * @brief Recursive divide and conquer helper
 *
 * Assumes points are sorted by x-coordinate.
 *
 * @param points_x Points sorted by x-coordinate
 * @param points_y Points sorted by y-coordinate (maintained for efficiency)
 * @param comparisons Counter for number of distance comparisons
 * @return Closest pair in the given set
 */
ClosestPairResult closest_pair_recursive(
    std::vector<Point>& points_x,
    std::vector<Point>& points_y,
    int& comparisons
) {
    int n = points_x.size();

    // Base case: use brute force for small instances
    if (n <= 3) {
        return brute_force_closest_pair_impl(points_x, comparisons);
    }

    // Divide: Find middle point
    int mid = n / 2;
    Point mid_point = points_x[mid];

    // Create left and right halves sorted by y
    std::vector<Point> left_y, right_y;
    for (const auto& p : points_y) {
        if (p.x < mid_point.x || (p.x == mid_point.x && p.y < mid_point.y)) {
            left_y.push_back(p);
        } else {
            right_y.push_back(p);
        }
    }

    // Create left and right halves for x
    std::vector<Point> left_x(points_x.begin(), points_x.begin() + mid);
    std::vector<Point> right_x(points_x.begin() + mid, points_x.end());

    // Conquer: Recursively find closest pair in each half
    ClosestPairResult left_result = closest_pair_recursive(left_x, left_y, comparisons);
    ClosestPairResult right_result = closest_pair_recursive(right_x, right_y, comparisons);

    // Find minimum from both halves
    ClosestPairResult best_result = (left_result.distance < right_result.distance)
                                     ? left_result : right_result;
    double delta = best_result.distance;

    // Combine: Check points in strip around dividing line
    std::vector<Point> strip;
    for (const auto& p : points_y) {
        if (std::abs(p.x - mid_point.x) < delta) {
            strip.push_back(p);
        }
    }

    // Find closest pair in strip
    if (!strip.empty()) {
        ClosestPairResult strip_result = find_strip_closest(strip, delta, comparisons);
        if (strip_result.distance < best_result.distance) {
            best_result = strip_result;
        }
    }

    return best_result;
}

/**
 * @brief Public interface for divide and conquer closest pair
 */
ClosestPairResult divide_conquer_closest_pair(std::vector<Point>& points) {
    if (points.size() < 2) {
        // Handle edge case: need at least 2 points
        ClosestPairResult result;
        result.distance = std::numeric_limits<double>::infinity();
        result.runtime_ms = 0;
        result.comparisons = 0;
        return result;
    }

    Timer timer;
    timer.start();

    int comparisons = 0;

    // Sort points by x and y coordinates (O(n log n))
    std::vector<Point> points_x = points;
    std::vector<Point> points_y = points;

    std::sort(points_x.begin(), points_x.end(), compare_x);
    std::sort(points_y.begin(), points_y.end(), compare_y);

    // Run divide and conquer
    ClosestPairResult result = closest_pair_recursive(points_x, points_y, comparisons);

    timer.stop();
    result.runtime_ms = timer.elapsed_ms();
    result.comparisons = comparisons;

    return result;
}

/**
 * @brief Public interface for brute force closest pair
 */
ClosestPairResult brute_force_closest_pair(const std::vector<Point>& points) {
    if (points.size() < 2) {
        // Handle edge case
        ClosestPairResult result;
        result.distance = std::numeric_limits<double>::infinity();
        result.runtime_ms = 0;
        result.comparisons = 0;
        return result;
    }

    Timer timer;
    timer.start();

    int comparisons = 0;
    ClosestPairResult result = brute_force_closest_pair_impl(points, comparisons);

    timer.stop();
    result.runtime_ms = timer.elapsed_ms();
    result.comparisons = comparisons;

    return result;
}
