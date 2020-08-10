

#CXXFLAGS = --std=c++17 -Wall
#CXX = g++ -O3

CXXFLAGS = -Wall --std=c++17
CXX = clang++ -O3

OBJS = bf_main.o bf_instruction.o bf_io.o bf_optimizations.o


bf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

profiler: clean
	$(CXX) $(CXXFLAGS) -DPROFILER bf*cpp -o $@

test: bf
	./bf < /dev/null && time ./bf mandelbrot.bf

clean:
	rm -rf a.out* bf *.o *.out 
