# Project Plan: Location-Based Social Network Algorithm Analysis

## Overview
This project implements and analyzes two fundamental algorithmic paradigms applied to a location-based social network where users post their locations and become friends when they visit places within a certain radius of each other.

## Problem 1: Greedy Algorithm - Friend Recommendation System

### Real-World Problem
Given a user with limited "follow slots" (e.g., only 10 premium follows), recommend which friends to follow to maximize connection quality based on shared location history.

### Problem Abstraction
- **Data Structure**: Weighted graph
- **Nodes**: Users in the social network
- **Edge Weights**: Number of shared locations or proximity score between users
- **Constraint**: Select at most k friends to follow
- **Objective**: Maximize total connection quality (sum of edge weights)

This can be formulated as a weighted set cover or maximum weighted matching problem.

### Solution Components
1. **Algorithm**: Greedy selection
   - Iteratively select the friend with highest marginal gain
   - At each step, pick the user that adds maximum value to current selection
   - Continue until k friends are selected

2. **Time Complexity**: O(n² log k) or O(n log n)
   - Depends on implementation details (priority queue vs simple iteration)

3. **Proof of Correctness**:
   - Show greedy achieves approximation guarantee
   - Prove that greedy gives at least (1-1/e) ≈ 63% of optimal solution
   - Use diminishing returns property of submodular functions

4. **Domain Explanation**:
   - Explain in terms of social network recommendations
   - Show how shared locations translate to friendship quality

5. **Experimental Validation**:
   - C++ implementation with timing analysis
   - Test on synthetic data with varying n and k
   - Plot runtime vs input size
   - Compare with brute force on small inputs to verify correctness

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
│   │   ├── friend_recommendation.cpp
│   │   └── friend_recommendation.h
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
- Implement friend recommendation algorithm in C++
- Write time complexity analysis
- Develop correctness proof
- Run experiments and collect data
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
- Use adjacency matrix or adjacency list for graph representation
- Implement priority queue for efficient friend selection
- Track already-selected friends to avoid duplicates
- Calculate marginal gain efficiently

### Divide & Conquer Algorithm Specifics
- Pre-sort points by x-coordinate once
- Maintain y-sorted order during recursion (avoid re-sorting)
- Strip search optimization: only check 7 points per point
- Handle edge cases: n ≤ 3, duplicate points

### Experimental Setup
- Generate random 2D points (uniform distribution in square region)
- For greedy: generate random weighted graphs
- Test sizes: n = 100, 500, 1000, 5000, 10000, 50000, 100000
- Run multiple trials (10-20) and report average
- Use high-resolution timer (chrono library in C++)
- Generate plots using Python/matplotlib or gnuplot

## LaTeX Template Recommendations
- ACM SIGPLAN template (good for algorithms)
- IEEE conference template
- Use algorithm2e or algorithmicx package for pseudocode
- Use TikZ for diagrams if needed
- Use pgfplots for plots (or import from matplotlib)

## Resources to Cite
- Introduction to Algorithms (CLRS) for standard algorithms
- Computational Geometry textbook for closest pair
- Original papers on greedy approximation algorithms
- Any online resources used for understanding proofs

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
