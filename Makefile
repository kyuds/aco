CC := g++

FLAGS := -std=c++20

MAIN_DIR := src
MAIN_FILES := $(wildcard $(MAIN_DIR)/*.cc)
MAIN_HEADERS := $(wildcard $(MAIN_DIR)/*.hh)

OUT_FILE := acotsp

acotsp: $(MAIN_FILES) $(MAIN_HEADERS)
	$(CC) $(FLAGS) $(MAIN_FILES) -o $(OUT_FILE)

clean:
	rm -f $(OUT_FILE)