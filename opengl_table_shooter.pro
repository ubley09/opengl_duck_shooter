TEMPLATE = app
LIBS += -lglut -lGLU -lGL -lGLEW -lglfw -lfreeimage -lSOIL -lSDL2main -lSDL2 -lSDL2_mixer
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Camera.cpp \
        main.cpp \
        object.cpp \
        scene.cpp \
        timer.cpp

HEADERS += \
    Camera.h \
    object.h \
    scene.h \
    timer.h
