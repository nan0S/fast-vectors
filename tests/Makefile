TARGET = tsv_test
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Ofast -march=native -flto
OBJS = $(TARGET).o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm $(TARGET) *.o

.PHONY: clean
