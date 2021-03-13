CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Ofast -march=native -flto -s
TARGET = tsv_test
OBJS = $(TARGET).o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -rf $(TARGET) *.o

.PHONY: clean
