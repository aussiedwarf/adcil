TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += include


SOURCES +=  test.c \
            source/adcil.c \
            source/adbuffer.c \
            source/adbmp.c \
            source/adpalette.c
            


HEADERS +=  include/adcil.h \
            include/adcil_types.h \
            include/adbuffer.h \
            include/adbmp.h \
            include/adpalette.h
            
