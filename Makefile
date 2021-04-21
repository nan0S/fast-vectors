CXX := g++ -std=c++17
# check c++20, but write for at least c++17 compatibility
# CXX := g++ -std=c++20

OFLAGS := -Ofast -march=native -flto -fomit-frame-pointer -s -DNDEBUG
WFLAGS := -Wall -Wextra
IFLAGS := -I include -I src -I include/container
CXXFLAGS := $(IFLAGS) $(OFLAGS) $(WFLAGS)

SRC_DIR := src
BUILD_DIR := build
TARGET_DIR := bin
TEST_DIR := test

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SOURCES))

TESTS := $(shell find $(TEST_DIR) -name '*.cpp')
TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(TARGET_DIR)/%,$(TESTS))

.PHONY: run all format clean
.SILENT: run

run: all
	for target in $(TARGETS); do \
		./$$target; \
	done

all: test

test: $(OBJECTS) $(TARGETS)

-include $(DEPENDS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(TARGET_DIR)/%: $(TEST_DIR)/%.cpp Makefile
	mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

format:
	@(shopt -s nullglob; \
	  clang-format -i *.cpp *.hpp *.c *.h)

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DIR)
