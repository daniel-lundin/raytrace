CXX = g++
CFLAGS = 

OBJECTS = bmp.o \
	  intersection.o \
	  parser.o \
	  parseentity.o \
	  pointlight.o \
	  raycamera.o \
	  raycanvas.o \
	  raycolor.o \
	  rayobject.o \
          raymaterial.o \
	  rayplane.o \
	  raysphere.o \
	  raycylinder.o \
	  raytriangle.o \
	  rayworld.o \
	  vector3d.o \
	  difference.o \
	  translation.o \
	  consolelogger.o \
	  utils.o

# Linking
raytrace : main.cpp $(OBJECTS)
	$(CXX) $(CFLAGS) main.cpp $(OBJECTS) -o raytrace

bmp.o: bmp.cpp bmp.h
	$(CXX) $(CFLAGS) -c bmp.cpp

intersection.o: intersection.cpp intersection.h
	$(CXX) $(CFLAGS) -c intersection.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(CFLAGS) -c parser.cpp

parseentity.o: parseentity.cpp parser.h
	$(CXX) $(CFLAGS) -c parseentity.cpp

pointlight.o: pointlight.cpp pointlight.h
	$(CXX) $(CFLAGS) -c pointlight.cpp

raycamera.o: raycamera.cpp raycamera.h
	$(CXX) $(CFLAGS) -c raycamera.cpp

raycanvas.o: raycanvas.cpp raycanvas.h
	$(CXX) $(CFLAGS) -c raycanvas.cpp

raycolor.o: raycolor.cpp raycolor.h
	$(CXX) $(CFLAGS) -c raycolor.cpp

rayobject.o: rayobject.h 
	$(CXX) $(CFLAGS) -c rayobject.cpp

raymaterial.o: raymaterial.cpp raymaterial.h
	$(CXX) $(CFLAGS) -c raymaterial.cpp

rayplane.o: rayplane.cpp rayplane.h
	$(CXX) $(CFLAGS) -c rayplane.cpp

raysphere.o: raysphere.cpp raysphere.h
	$(CXX) $(CFLAGS) -c raysphere.cpp

raycylinder.o: raycylinder.cpp raycylinder.h
	$(CXX) $(CFLAGS) -c raycylinder.cpp

raytriangle.o: raytriangle.cpp raytriangle.h
	$(CXX) $(CFLAGS) -c raytriangle.cpp

rayworld.o: rayworld.cpp rayworld.h
	$(CXX) $(CFLAGS) -c rayworld.cpp

vector3d.o: vector3d.cpp vector3d.h
	$(CXX) $(CFLAGS) -c vector3d.cpp

difference.o: difference.cpp difference.h
	$(CXX) $(CFLAGS) -c difference.cpp

translation.o: translation.cpp translation.h
	$(CXX) $(CFLAGS) -c translation.cpp

consolelogger.o: consolelogger.cpp consolelogger.h
	$(CXX) $(CFLAGS) -c consolelogger.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CFLAGS) -c utils.cpp
clean:
	rm *.o
