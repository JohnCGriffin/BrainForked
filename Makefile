

#CXXFLAGS = --std=c++11 -Wall
#CXX = g++ -O3

CXXFLAGS = -Wall --std=c++11
CXX = clang++ -O3

OBJS = bf_main.o bf_instruction.o bf_io.o bf_optimizations.o


bf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

test: bf
	./bf < /dev/null && time ./bf mandelbrot.bf
	

clean:
	rm -rf a.out* bf *.o *.out 
