# Makefile for Location-Based Social Network Algorithms

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3
INCLUDES = -Isrc

# Source files
GREEDY_SOURCES = src/greedy/max_coverage.cpp
DIVIDE_CONQUER_SOURCES = src/divide_conquer/closest_pair.cpp
EXPERIMENT_SOURCES = experiments/run_experiments.cpp

# Output binaries
EXPERIMENT_BIN = experiments/run_experiments

# Targets
.PHONY: all clean experiments run plots help

all: experiments

# Compile experiment runner
experiments: $(EXPERIMENT_BIN)

$(EXPERIMENT_BIN): $(EXPERIMENT_SOURCES) $(GREEDY_SOURCES) $(DIVIDE_CONQUER_SOURCES)
	@echo "Compiling experiment runner..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^
	@echo "Done! Binary: $(EXPERIMENT_BIN)"

# Run experiments (generates CSV files)
run: experiments
	@echo "Running experiments..."
	@echo "This may take a few minutes..."
	./$(EXPERIMENT_BIN)
	@echo ""
	@echo "Experiments complete! Data saved in experiments/data/"

# Generate plots from CSV data
plots: run
	@echo "Generating plots..."
	@chmod +x experiments/plot_results.py
	python3 experiments/plot_results.py
	@echo ""
	@echo "Plots saved in experiments/plots/"

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -f $(EXPERIMENT_BIN)
	rm -f experiments/data/*.csv
	rm -f experiments/plots/*.png
	rm -f experiments/plots/*.pdf
	@echo "Clean complete!"

# Help
help:
	@echo "Available targets:"
	@echo "  make all        - Compile all code (default)"
	@echo "  make experiments - Compile experiment runner"
	@echo "  make run        - Run experiments (generates CSV data)"
	@echo "  make plots      - Run experiments and generate plots"
	@echo "  make clean      - Remove all generated files"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Quick start:"
	@echo "  make plots      - Does everything (compile, run, plot)"
