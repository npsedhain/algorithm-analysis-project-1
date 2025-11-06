# Project Plan: Location-Based Social Network Algorithm Analysis

## Overview
This project implements and analyzes two fundamental algorithmic paradigms applied to a location-based social network where users post their locations and become friends when they visit places within a certain radius of each other.

## Problem 1: Greedy Algorithm - Maximum Location Discovery

### Real-World Problem
You just moved to a new city and want to explore interesting places. You can follow at most k local friends who will share their favorite locations with you. Each friend has visited different sets of locations (restaurants, parks, museums, etc.). Which k friends should you follow to discover the maximum number of unique places?

**Example**: If Alice visited {Starbucks, Central Park, MoMA} and Bob visited {Central Park, Brooklyn Bridge, Times Square}, following both gives you 5 unique locations (Central Park appears in both but counts once).

### Problem Abstraction
- **Input**:
  - Set of users U = {u₁, u₂, ..., uₙ}
  - Each user uᵢ has visited a set of locations Lᵢ ⊆ L (where L is the set of all locations)
  - Constraint: Select at most k users
- **Objective**: Maximize |⋃_{u ∈ S} Lᵤ| (total number of unique locations discovered)
- **Problem Type**: Maximum Coverage Problem (NP-hard)

This is a classic submodular optimization problem with well-studied theoretical properties.

### Solution Components
1. **Algorithm**: Greedy Maximum Coverage
   - **Initialization**: S = ∅ (selected friends), covered = ∅ (discovered locations)
   - **Iteration** (repeat k times):
     - For each unselected user u, compute marginal gain: |Lᵤ \ covered|
     - Select user u* with maximum marginal gain
     - Add u* to S and update covered locations
   - **Output**: Selected set S and total coverage |covered|

   **Pseudocode**:
   ```
   GreedyMaxCoverage(users, k):
       S = ∅, covered = ∅
       for i = 1 to k:
           best_user = argmax_{u ∉ S} |Lᵤ \ covered|
           S = S ∪ {best_user}
           covered = covered ∪ L_best_user
       return S, |covered|
   ```

2. **Time Complexity**: O(n·k·m) where m = average locations per user
   - Outer loop: k iterations
   - Inner loop: scan n users
   - Marginal gain computation: O(m) per user (set difference)
   - **Optimization**: Can be improved to O(n·k·log m) with better data structures

3. **Proof of Correctness**:
   - **Step 1**: Define objective function f(S) = |⋃_{u∈S} Lᵤ|
   - **Step 2**: Prove f is monotone (adding users never decreases coverage)
   - **Step 3**: Prove f is submodular (diminishing returns property):
     - For sets A ⊆ B and element u: f(A ∪ {u}) - f(A) ≥ f(B ∪ {u}) - f(B)
     - Intuition: Adding u to smaller set A gives more new locations than adding to larger set B
   - **Step 4**: Apply submodular maximization theorem:
     - **Theorem**: For monotone submodular f, greedy achieves f(S_greedy) ≥ (1 - 1/e) · f(S_opt)
   - **Step 5**: Prove by induction using marginal gain analysis
   - **Result**: Greedy guarantees at least 63% of optimal coverage

4. **Domain Explanation**:
   - Models real location discovery through social connections
   - Captures diminishing returns: early friends add many new places, later friends have more overlap
   - Useful for recommendation systems, city guides, travel apps
   - Trade-off between quantity (many friends) and constraint (limited follows)

5. **Experimental Validation**:
   - C++ implementation with timing analysis
   - **Synthetic Data Generation**:
     - n users, each visits m_avg locations (from pool of L total locations)
     - Random location assignment or clustered (some locations more popular)
   - **Test Parameters**:
     - n ∈ {100, 500, 1000, 5000, 10000}
     - k ∈ {5, 10, 20, 50}
     - L = 1000-10000 locations
   - **Comparisons**:
     - Greedy vs Brute Force (for small k ≤ 10)
     - Greedy vs Random Selection
     - Verify approximation ratio ≥ 63%
   - **Plots**:
     - Runtime vs n (show linear in n for fixed k)
     - Coverage vs k (show diminishing returns)
     - Approximation ratio (greedy/optimal) vs problem size

## Problem 2: Divide & Conquer - Closest Pair of Users

### Real-World Problem
Find two users who have been geographically closest to each other based on their location posts. This identifies potential "near miss" connections - people who were very close but haven't connected yet.

### Problem Abstraction
- **Data Structure**: Set of points in 2D plane
- **Points**: User locations as (x, y) coordinates (latitude, longitude)
- **Distance Metric**: Euclidean distance
- **Objective**: Find the pair of points with minimum distance

Classic closest pair of points problem in computational geometry.

### Solution Components
1. **Algorithm**: Divide and Conquer approach
   - **Divide**: Sort points by x-coordinate, split into left and right halves
   - **Conquer**: Recursively find closest pair in each half
   - **Combine**: Check pairs across the dividing line within distance δ (min of two halves)
   - **Optimization**: Only check points in vertical strip, sorted by y-coordinate

2. **Time Complexity**: O(n log n)
   - T(n) = 2T(n/2) + O(n)
   - Initial sort: O(n log n)
   - Recurrence solves to O(n log n) by Master Theorem

3. **Proof of Correctness**:
   - Prove by induction on n
   - Base case: n ≤ 3 (brute force)
   - Inductive step: Show that checking strip of width 2δ is sufficient
   - Prove at most 7 comparisons needed per point in strip

4. **Domain Explanation**:
   - Explain as finding users who were physically closest
   - Useful for "you just missed each other" notifications
   - Can suggest connections based on near encounters

5. **Experimental Validation**:
   - C++ implementation with precise timing
   - Compare with O(n²) brute force algorithm
   - Test on synthetic location data (random points in bounded region)
   - Plot runtime vs n on log scale
   - Show empirical O(n log n) behavior

## Project Structure

```
project-1/
├── plan.md                          # This file
├── README.md                        # Project overview and setup instructions
├── report/
│   ├── main.tex                     # Main LaTeX document
│   ├── sections/
│   │   ├── introduction.tex
│   │   ├── greedy_problem.tex
│   │   ├── divide_conquer_problem.tex
│   │   ├── experiments.tex
│   │   └── conclusion.tex
│   ├── figures/                     # Experimental plots
│   ├── appendices/
│   │   ├── code.tex                 # Algorithm implementations
│   │   └── llm_usage.tex           # LLM prompts and outputs
│   └── references.bib               # Bibliography
├── src/
│   ├── greedy/
│   │   ├── max_coverage.cpp
│   │   └── max_coverage.h
│   ├── divide_conquer/
│   │   ├── closest_pair.cpp
│   │   └── closest_pair.h
│   ├── common/
│   │   ├── timer.h                  # Timing utilities
│   │   └── data_generator.cpp       # Synthetic data generation
│   └── main.cpp                     # Test driver
├── experiments/
│   ├── run_experiments.cpp          # Experiment runner
│   ├── data/                        # Raw timing data (CSV)
│   └── plots/                       # Generated graphs
├── Makefile                         # Build system
└── .gitignore
```

## Implementation Timeline

### Phase 1: Setup (Day 1-2)
- Set up C++ project structure
- Create Makefile for compilation
- Implement timing utilities
- Create synthetic data generators
- Set up LaTeX project using ACM/IEEE template on Overleaf

### Phase 2: Greedy Algorithm (Day 3-5)
- Implement maximum coverage algorithm in C++
- Implement brute force baseline for small inputs
- Write time complexity analysis (O(n·k·m))
- Develop submodularity proof and approximation guarantee
- Run experiments: greedy vs optimal vs random
- Measure approximation ratio (verify ≥ 63%)
- Write corresponding LaTeX sections

### Phase 3: Divide & Conquer Algorithm (Day 6-8)
- Implement closest pair algorithm in C++
- Implement brute force for comparison
- Write time complexity analysis with Master Theorem
- Develop correctness proof by induction
- Run experiments and generate comparison plots
- Write corresponding LaTeX sections

### Phase 4: Report Writing (Day 9-12)
- Complete introduction and problem descriptions
- Finalize all proofs and analyses
- Generate final experimental plots
- Write experimental results section
- Add code to appendices
- Document all LLM usage with prompts
- Add proper citations
- Proofread and format

### Phase 5: Final Review (Day 13-14)
- Verify all proofs are correct
- Check experimental results match theoretical analysis
- Ensure code compiles and runs correctly
- Final LaTeX formatting
- Prepare submission

## Key Technical Details

### Greedy Algorithm Specifics
- **Data Structure**:
  - Use `std::unordered_set<int>` for each user's location set (fast set operations)
  - Use `std::unordered_set<int>` for tracking covered locations
  - Use `std::vector<bool>` to mark selected users
- **Optimization Techniques**:
  - Cache marginal gains and update incrementally (only recompute for affected users)
  - Use bitmasks for location sets if |L| is small (fast set operations)
  - Consider lazy evaluation: don't recompute all gains every iteration
- **Marginal Gain Computation**:
  - For each user u: count locations in Lᵤ not in covered set
  - Efficient set difference: iterate through u's locations, check membership
- **Implementation Variants**:
  - Basic: O(n·k·m) - recompute all gains each iteration
  - Optimized: O(n·k·log m) - use priority queue with lazy updates
- **Testing Strategy**:
  - Unit test: verify submodular property on small examples
  - Correctness: compare with brute force on small inputs (k ≤ 10, n ≤ 20)
  - Approximation ratio: measure f(S_greedy)/f(S_optimal) ≥ 0.63

### Divide & Conquer Algorithm Specifics
- Pre-sort points by x-coordinate once
- Maintain y-sorted order during recursion (avoid re-sorting)
- Strip search optimization: only check 7 points per point
- Handle edge cases: n ≤ 3, duplicate points

### Experimental Setup
- **Greedy Algorithm Data**:
  - Generate n users, each with random set of locations
  - Location pool size: L = 1000-10000 locations
  - Each user visits m_avg ≈ 20-100 locations (randomly sampled)
  - Can use Zipf distribution for realistic popularity (some locations visited by many users)
  - Test sizes: n ∈ {100, 500, 1000, 5000, 10000}
  - Test k values: k ∈ {5, 10, 20, 50}
- **Divide & Conquer Data**:
  - Generate random 2D points (uniform distribution in square region)
  - Test sizes: n = 100, 500, 1000, 5000, 10000, 50000, 100000
- **Timing**:
  - Run multiple trials (10-20) and report average
  - Use high-resolution timer (chrono library in C++)
- **Plotting**:
  - Generate plots using Python/matplotlib or gnuplot
  - Save raw data as CSV for reproducibility

## LaTeX Template Recommendations
- ACM SIGPLAN template (good for algorithms)
- IEEE conference template
- Use algorithm2e or algorithmicx package for pseudocode
- Use TikZ for diagrams if needed
- Use pgfplots for plots (or import from matplotlib)

## Resources to Cite
- Introduction to Algorithms (CLRS) for standard algorithms
- Computational Geometry textbook (O'Rourke or de Berg) for closest pair
- **Greedy/Submodular Optimization**:
  - Nemhauser, Wolsey, Fisher (1978): "An analysis of approximations for maximizing submodular set functions"
  - Feige (1998): "A threshold of ln n for approximating set cover"
  - Krause & Golovin (2014): "Submodular Function Maximization" survey
- Any online resources used for understanding proofs (with proper attribution)

## LLM Usage Documentation (Appendix)
Document in the appendix:
1. All prompts given to LLMs
2. Full responses received
3. How the responses were used
4. What was modified from LLM output
5. Verification steps taken to ensure correctness

## Success Criteria
- Both algorithms correctly implemented and tested
- Time complexity matches theoretical analysis in experiments
- Proofs are rigorous and correct
- Report is well-written and professionally formatted
- Code is clean, documented, and runs correctly
- All requirements from assignment met (100 pts total)

## Notes
- Language: C++
- Experimental Data: Synthetic (randomly generated)
- Theory Level: Clear explanations with detailed proofs
- Focus on correctness and clarity over optimization
