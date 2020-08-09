

#CXXFLAGS = --std=c++11 -Wall
#CXX = g++ -O3

CXXFLAGS = -Wall --std=c++11
CXX = clang++ -O3

OBJS = bf.o bf_instruction.o bf_io.o bf_optimizations.o


bf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

clean:
	rm -rf a.out* bf *.o *.out 
