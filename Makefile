CXX := g++ -std=c++17
# check c++20, but write for at least c++17 compatibility
# CXX := g++ -std=c++20

OFLAGS := -Ofast -march=native -flto -fomit-frame-pointer -s -DNDEBUG
WFLAGS := -Wall -Wextra
IFLAGS := -I include -I src -I include/container
DFLAGS := -ggdb -fsanitize=address
CXXFLAGS := $(IFLAGS) $(OFLAGS) $(WFLAGS)

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test
INSTALL_DIR := /usr/local/include

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
TSOURCES := $(shell find $(TEST_DIR) -name '*.cpp')

OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TSOURCES))

DEPENDS := $(patsubst %.cpp,$(BUILD_DIR)/%.d,$(SOURCES))
DEPENDS += $(patsubst %.cpp,$(BUILD_DIR)/%.d,$(TSOURCES))

TESTS := $(filter $(BIN_DIR)/tests/%,$(TARGETS))
BENCHMARKS := $(filter $(BIN_DIR)/benchmarks/%, $(TARGETS))

.PHONY: clean
.SECONDARY: $(patsubst %.d,%.o,$(DEPENDS)) # do not remove %.o files

all: $(TARGETS)

run-benchmarks: $(BENCHMARKS)
	@for benchmark in $(BENCHMARKS); do \
		./$$benchmark; \
		echo; \
	done
	
run-tests: $(TESTS)
	@for test in $(TESTS); do \
		./$$test; \
	done

run-%: $(BIN_DIR)/benchmarks/%
	@./$<
run-%: $(BIN_DIR)/tests/%
	@./$<

-include $(DEPENDS)

$(BIN_DIR)/tests/%: $(BUILD_DIR)/$(TEST_DIR)/tests/%.o $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -lgtest -MMD -MP $< $(OBJECTS) -o $@

$(BIN_DIR)/benchmarks/%: $(BUILD_DIR)/$(TEST_DIR)/benchmarks/%.o $(OBJECTS) 
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -MP $< $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.cpp 
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

install:
	@echo Installing ...
	@rm -rf $(INSTALL_DIR)/uwr
	@cp -r include $(INSTALL_DIR)/uwr
	@echo Done

uninstall:
	@echo Uninstalling ...
	@rm -rf $(INSTALL_DIR)/uwr
	@echo Done

format:
	@(shopt -s nullglob; \
	  clang-format -i *.cpp *.hpp *.c *.h)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
