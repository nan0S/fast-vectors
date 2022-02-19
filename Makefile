INSTALL_DIR := /usr/local/include

CXX      := g++
# VERSION  := -std=c++17
# check c++20, but write for at least c++17 compatibility
VERSION  := -std=c++20
CXXFLAGS    := $(VERSION) -Wall -Wextra
# BENCH_FLAGS := -O3 -DNDEBUG
BENCH_FLAGS := -O0
TEST_FLAGS  := -O0 -ggdb -fno-omit-frame-pointer -fno-inline-functions

INC := -Isrc -Iinclude
LIB := -pthread -lgtest -lbenchmark -lEASTL -lfolly -ldl -lfmt -ljemalloc

SRC  	  := $(shell find src -name '*.cpp')
OBJ 	  := $(patsubst %.cpp, build/%.o, $(SRC))
TSRC    := $(shell find test -name '*.cpp')
TARGETS := $(patsubst test/%.cpp, bin/%, $(TSRC))
DEP     := $(patsubst %.cpp, build/%.d, $(SRC)) + \
			  $(patsubst %.cpp, build/%.d, $(TSRC))
TEST    := $(filter bin/tests/%, $(TARGETS))
TEST    := $(filter-out bin/tests/temp_test, $(TEST)) # filter out scratchpad test
BENCH   := $(filter bin/benchmarks/%, $(TARGETS))

.PHONY: clean
.SECONDARY: $(patsubst %.d,%.o,$(DEP)) # do not remove %.o files

all: $(TARGETS)

run-all: run-benchmarks run-tests

run-benchmarks: $(BENCH)
	@for benchmark in $(BENCH); do \
		./$$benchmark --benchmark_color=yes --benchmark_counters_tabular=true; \
		echo; \
	done
	
run-tests: $(TEST)
	@for test in $(TEST); do \
		./$$test --gtest_color=yes; \
	done

run-%: bin/benchmarks/%
	@./$< --benchmark_color=yes --benchmark_counters_tabular=true
run-%: bin/tests/%
	@./$< --gtest_color=yes

-include $(DEP)

bin/tests/%: build/test/tests/%.o $(OBJ)
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) -MMD -MP $< $(OBJ) -o $@ $(LIB)

bin/benchmarks/%: build/test/benchmarks/%.o $(OBJ)
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(BENCH_FLAGS) -MMD -MP $< $(OBJ) -o $@ $(LIB)

build/%.o: %.cpp Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(BENCH_FLAGS) -MMD -MP -c $< -o $@ $(INC)

build/test/tests/%.o: test/tests/%.cpp Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) -MMD -MP -c $< -o $@ $(INC)

install:
	@echo Installing to $(INSTALL_DIR)/uwr.
	@rm -rf $(INSTALL_DIR)/uwr
	@cp -r include/uwr $(INSTALL_DIR)
	@echo Done.

uninstall:
	@echo Uninstalling from $(INSTALL_DIR)/uwr.
	@rm -rf $(INSTALL_DIR)/uwr
	@echo Done.

format:
	@(shopt -s nullglob; \
	  clang-format -i *.cpp *.hpp *.c *.h)

clean:
	rm -rf bin build compile_commands.json
