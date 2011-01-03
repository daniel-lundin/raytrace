CXX = g++

OBJECTS = bmp.o \
	  intersection.o \
	  parser.o \
	  pointlight.o \
	  raycamera.o \
	  raycanvas.o \
	  raycolor.o \
	  rayobject.o \
          raymaterial.o \
	  rayplane.o \
	  raysphere.o \
	  raytriangle.o \
	  rayworld.o \
	  vector3d.o

# Linking
raytrace : main.cpp $(OBJECTS)
	$(CXX) main.cpp $(OBJECTS) -o raytrace

bmp.o: bmp.cpp bmp.h
	$(CXX) -c bmp.cpp

intersection.o: intersection.cpp intersection.h
	$(CXX) -c intersection.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

pointlight.o: pointlight.cpp pointlight.h
	$(CXX) -c pointlight.cpp

raycamera.o: raycamera.cpp raycamera.h
	$(CXX) -c raycamera.cpp

raycanvas.o: raycamera.cpp raycamera.h
	$(CXX) -c raycamera.cpp

raycolor.o: raycolor.cpp raycolor.h
	$(CXX) -c raycolor.cpp

rayobject.o: rayobject.h 
	$(CXX) -c rayobject.cpp

raymaterial.o: raymaterial.cpp raymaterial.h
	$(CXX) -c raymaterial.cpp

rayplane.o: rayplane.cpp rayplane.h
	$(CXX) -c rayplane.cpp

raysphere.o: raysphere.cpp raysphere.h
	$(CXX) -c raysphere.cpp

raytriangle.o: raytriangle.cpp raytriangle.h
	$(CXX) -c raytriangle.cpp

rayworld.o: rayworld.cpp rayworld.h
	$(CXX) -c rayworld.cpp

vector3d.o: vector3d.cpp vector3d.h
	$(CXX) -c vector3d.cpp
