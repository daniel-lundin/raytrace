CXX 	= g++
CXXFLAGS = -Wall -DNUM_THREADS=2 -O2
LIBS = -lpthread

allobjs = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
mainobjs = $(filter-out src/testmain.o, $(allobjs))
testobjs = $(filter-out src/main.o, $(allobjs))

raytrace: $(mainobjs)
	$(CXX) $(CFLAGS) $(mainobjs) -o raytrace $(LIBS)

test: $(testobjs)
	$(CXX) $(CFLAGS) $(testobjs) -o test $(LIBS)

clean:
	rm -f src/*.o
