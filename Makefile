CXX 	= g++
CXXFLAGS = -Wall -DNUM_THREADS=1 -O2

allobjs = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
mainobjs = $(filter-out src/testmain.o, $(allobjs))
testobjs = $(filter-out src/main.o, $(allobjs))

raytrace: $(mainobjs)
	$(CXX) $(CFLAGS) $(mainobjs) -o raytrace 

test: $(testobjs)
	$(CXX) $(CFLAGS) $(testobjs) -o test

clean:
	rm -f src/*.o
