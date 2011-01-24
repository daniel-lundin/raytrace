CXX 	= g++
CXXFLAGS = -Wall -DNUM_THREADS=2

objects = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
raytrace: $(objects) 
	$(CXX) $(CFLAGS) $(objects) -o raytrace 

clean:
	rm -f src/*.o
