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
TARGET_DIR := bin
TEST_DIR := test

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SOURCES))

TESTS := $(shell find $(TEST_DIR) -name '*.cpp')
TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(TARGET_DIR)/%,$(TESTS))
DEPENDS += $(patsubst $(TEST_DIR)/%.cpp,$(TARGET_DIR)/%.d,$(TESTS))

INSTALLDIR := /usr/local/include

.PHONY: run all format clean install uninstall
.SILENT: run

run: all
	for target in $(TARGETS); do \
		./$$target; \
	done

run-%: $(TARGET_DIR)/%
	./$<

all: test

test: $(TARGETS)

-include $(DEPENDS)

$(TARGET_DIR)/%: $(TEST_DIR)/%.cpp $(OBJECTS) Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ $< $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

install:
	@echo Installing ...
	@rm -rf $(INSTALLDIR)/uwr
	@cp -r include $(INSTALLDIR)/uwr
	@echo Done

uninstall:
	@echo Uninstalling ...
	@rm -rf $(INSTALLDIR)/uwr
	@echo Done

format:
	@(shopt -s nullglob; \
	  clang-format -i *.cpp *.hpp *.c *.h)

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DIR)
