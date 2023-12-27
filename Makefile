CC := g++

FLAGS := -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY

SUDOKU_DIR := src/sudoku
SUDOKU_FILES := $(wildcard $(SUDOKU_DIR)/*.cc)
SUDOKU_HEADERS := $(wildcard $(SUDOKU_DIR)/*.hh)

TSP_DIR := src/tsp
TSP_FILES := $(wildcard $(TSP_DIR)/*.cc)
TSP_HEADERS := $(wildcard $(TSP_DIR)/*.hh)

OUT_FILE_SUDOKU := acosudoku
OUT_FILE_TSP := acotsp

help:
	@echo "Enter 'make sudoku' or 'make tsp' to build for either optimization."

sudoku: $(SUDOKU_FILES) $(SUDOKU_HEADERS)
	$(CC) $(FLAGS) $(SUDOKU_FILES) -o $(OUT_FILE_SUDOKU)

tsp: $(TSP_FILES) $(TSP_HEADERS)
	$(CC) $(FLAGS) $(TSP_FILES) -o $(OUT_FILE_TSP)

clean:
	rm -rf $(OUT_FILE_SUDOKU)
	rm -rf $(OUT_FILE_TSP)
