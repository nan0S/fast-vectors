CXX := g++
CXXFLAGS := -std=c++17 -Ofast -march=native -flto -s -DNDEBUG
# check c++20, but write for at least c++17 compatibility
# CXXFLAGS := -std=c++20 -Ofast -march=native -flto -s -DNDEBUG
WARNING := -Wall -Wextra

TARGET := main

SOURCES := main.cpp test_type.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

.PHONY: all clean

run: all
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(WARNING) $(CXXFLAGS) -o $@ $^

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c -o $@ $<

format:
	@(shopt -s nullglob; \
	  clang-format -i *.cpp *.hpp *.c *.h)

clean:
	rm -rf $(TARGET) $(OBJECTS) $(DEPENDS)
