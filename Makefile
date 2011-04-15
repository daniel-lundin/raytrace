CXX 	= g++
CXXFLAGS = -Wall -O2 #-ggdb #-pg
#CXXFLAGS += -DNUM_THREADS=2
LIBS = -lpthread 
LDFLAGS = #-pg

allobjs = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
mainobjs = $(filter-out src/testmain.o, $(allobjs))
testobjs = $(filter-out src/main.o, $(allobjs))

raytrace: $(mainobjs)
	$(CXX) $(CFLAGS) $(mainobjs) -o raytrace $(LIBS) $(LDFLAGS)

test: $(testobjs)
	$(CXX) $(CFLAGS) $(testobjs) -o test $(LIBS)

clean:
	rm -f src/*.o
