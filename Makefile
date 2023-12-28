CC := g++

FLAGS := -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY

DIR := src
FILES := $(wildcard $(TSP_DIR)/*.cc)
HEADERS := $(wildcard $(TSP_DIR)/*.hh)

OUT_FILE := acotsp

tsp: $(FILES) $(HEADERS)
	$(CC) $(FLAGS) $(FILES) -o $(OUT_FILE)

clean:
	rm -rf $(OUT_FILE)
