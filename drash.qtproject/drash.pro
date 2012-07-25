SOURCES += \
    ../src/main.cpp \
    ../src/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/cvec2.cpp \
    ../src/clogger.cpp

CONFIG += console

CONFIG -= qt

HEADERS += \
    ../src/csceneobject.h \
    ../src/cscene.h \
    ../src/cvec2.h \
    ../src/clogger.h

unix|win32: LIBS += -lBox2D
