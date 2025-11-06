#ifndef TIMER_H
#define TIMER_H

#include <chrono>

/**
 * @brief High-resolution timer for benchmarking
 *
 * Uses std::chrono for microsecond precision timing.
 */
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running;

public:
    Timer() : running(false) {}

    /**
     * @brief Start the timer
     */
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    /**
     * @brief Stop the timer
     */
    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

    /**
     * @brief Get elapsed time in milliseconds
     * @return Elapsed time in milliseconds (double precision)
     */
    double elapsed_ms() {
        if (running) {
            end_time = std::chrono::high_resolution_clock::now();
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        return duration.count() / 1000.0;  // Convert microseconds to milliseconds
    }

    /**
     * @brief Get elapsed time in seconds
     * @return Elapsed time in seconds (double precision)
     */
    double elapsed_s() {
        return elapsed_ms() / 1000.0;
    }
};

#endif // TIMER_H
