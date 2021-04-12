CXX := g++
CXXFLAGS := -std=c++17 -Ofast -march=native -flto -s -DNDEBUG
WARNING := -Wall -Wextra

TARGET := main

SOURCES := main.cpp test_type.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

.PHONY: all clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(WARNING) $(CXXFLAGS) -o $@ $^

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJECTS) $(DEPENDS)
