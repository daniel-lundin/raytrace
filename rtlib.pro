# -------------------------------------------------
# Project created by QtCreator 2010-11-26T17:18:23
# -------------------------------------------------
# QT -= gui
TARGET = rtlib
TEMPLATE = app # lib

# CONFIG += staticlib
SOURCES += rtlib.cpp \
    intersection.cpp \
    pointlight.cpp \
    raycamera.cpp \
    raycanvas.cpp \
    raycolor.cpp \
    raymaterial.cpp \
    rayobject.cpp \
    rayplane.cpp \
    raysphere.cpp \
    raytriangle.cpp \
    rayworld.cpp \
    vector3d.cpp \
    main.cpp \
    parser.cpp \
    bmp.cpp
HEADERS += rtlib.h \
    intersection.h \
    pointlight.h \
    raycamera.h \
    raycanvas.h \
    raycolor.h \
    raymaterial.h \
    rayobject.h \
    rayplane.h \
    raysphere.h \
    raytriangle.h \
    rayworld.h \
    vector3d.h \
    parser.h \
    bmp.h
OTHER_FILES += Makefile.txt
